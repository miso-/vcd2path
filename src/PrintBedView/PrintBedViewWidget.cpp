#include "PrintBedViewWidget.h"

#include "PrintBedView.h"
#include "../Vcd2path.h"
#include "../StepperDriver.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QGraphicsItem>
#include <QGroupBox>
#include <QRadioButton>

PrintBedViewWidget::PrintBedViewWidget(Vcd2path *vcd2path, QWidget *parent)
	: QWidget(parent),
	  m_printBedView(new PrintBedView(this)),
	  m_cbX(0),
	  m_cbY(0),
	  m_pbPlot(0),
	  m_vcd2path(vcd2path),
	  m_offRBtn(0), m_foregroundRBtn(0), m_backgroundRBtn(0)
{
	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->addWidget(m_printBedView);

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel("Plot"));
	m_cbX = new QComboBox();
	hLayout->addWidget(m_cbX);
	hLayout->addWidget(new QLabel("vs."));
	m_cbY = new QComboBox();
	hLayout->addWidget(m_cbY);
	m_pbPlot = new QPushButton("Ok");
	hLayout->addWidget(m_pbPlot);
	hLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

	QGroupBox *gridGrpBox = new QGroupBox("Grid");
	QHBoxLayout *gridGrpBoxLayout = new QHBoxLayout();
	m_offRBtn = new QRadioButton("Off");
	m_foregroundRBtn = new QRadioButton("Foreground");
	m_backgroundRBtn = new QRadioButton("BackGground");
	gridGrpBoxLayout->addWidget(m_offRBtn);
	gridGrpBoxLayout->addWidget(m_foregroundRBtn);
	gridGrpBoxLayout->addWidget(m_backgroundRBtn);
	gridGrpBox->setLayout(gridGrpBoxLayout);
	m_foregroundRBtn->setChecked(true);
	hLayout->addWidget(gridGrpBox);
	connect(m_offRBtn, SIGNAL(toggled(bool)), this, SLOT(on_gridRBtn_toggled()));
	connect(m_foregroundRBtn, SIGNAL(toggled(bool)), this, SLOT(on_gridRBtn_toggled()));
	connect(m_backgroundRBtn, SIGNAL(toggled(bool)), this, SLOT(on_gridRBtn_toggled()));

	vLayout->addLayout(hLayout);

	setLayout(vLayout);

	connect(m_pbPlot, SIGNAL(clicked(bool)), this, SLOT(on_pbPlot_clicked()));
}

PrintBedViewWidget::~PrintBedViewWidget()
{
	delete m_printBedView;
}

void PrintBedViewWidget::reloadData()
{
	if (!m_vcd2path) {
		return;
	}

	const QVector<QSharedPointer<StepperDriver> > &drivers = m_vcd2path->drivers();
	m_cbX->clear();
	m_cbY->clear();

	for (QVector<QSharedPointer<StepperDriver> >::const_iterator drvI = drivers.begin(); drvI != drivers.end(); ++drvI) {
		QString name = (*drvI)->name();
		m_cbX->addItem(name);
		m_cbY->addItem(name);
	}

	if (m_cbY->count() > 1) {
		m_cbY->setCurrentIndex(1);
	}

	drawData();
}

void PrintBedViewWidget::on_pbPlot_clicked()
{
	drawData();
}

void PrintBedViewWidget::on_gridRBtn_toggled()
{
	if (m_offRBtn->isChecked()) {
		m_printBedView->setGridPosition(PrintBedView::Off);
	} else if (m_foregroundRBtn->isChecked()) {
		m_printBedView->setGridPosition(PrintBedView::Foreground);
	} else if (m_backgroundRBtn->isChecked()) {
		m_printBedView->setGridPosition(PrintBedView::Background);
	}
}

void PrintBedViewWidget::drawData()
{
	if (!m_vcd2path) {
		return;
	}

	m_printBedView->clear();
	int xI = m_cbX->currentIndex();
	int yI = m_cbY->currentIndex();

	QPen threadPen;
	threadPen.setColor(Qt::blue);
	QPointF lastPos;
	for (QVector<PrinterState>::const_iterator i = m_vcd2path->stateTrace().begin(); i != m_vcd2path->stateTrace().end(); ++i) {
		if (xI >= i->position.size()) {
			xI = 0;
		}
		if (yI >= i->position.size()) {
			yI = 0;
		}

		QPointF currPos(i->position[xI], i->position[yI]);
		QGraphicsLineItem *l = new QGraphicsLineItem(QLineF(lastPos, currPos));
		lastPos = currPos;
		l->setPen(threadPen);
		l->setToolTip(QString("[%1:%2]-[%3:%4]\ntimestamp: %5").arg(lastPos.x()).arg(lastPos.y())
					  .arg(currPos.x()).arg(currPos.y()).arg(i->timestamp));
		m_printBedView->addItem(l);
	}
}
