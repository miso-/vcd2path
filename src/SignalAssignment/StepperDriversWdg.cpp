#include "StepperDriversWdg.h"

#include "../StepperDriver.h"
#include "StepperDriverTreeItem.h"

#include <QPushButton>
#include <QTreeWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStyledItemDelegate>
#include <QStringList>

class NotEditable: public QStyledItemDelegate
{
public:
	virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
		Q_UNUSED(parent)
		Q_UNUSED(option)
		Q_UNUSED(index)

		return 0;
	}
};

StepperDriversWdg::StepperDriversWdg(const QVector<QSharedPointer<StepperDriver> > &drivers, QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent, f),
	  m_pbUp(new QPushButton("Up", this)),
	  m_pbDown(new QPushButton("Down")),
	  m_pbAdd(new QPushButton("Add")),
	  m_pbRemove(new QPushButton("Remove")),
	  m_driversTree(new QTreeWidget()),
	  m_notEditable(new NotEditable())

{
	QStringList columns;
	columns << "Property" << "Value";
	m_driversTree->setHeaderLabels(columns);
	m_driversTree->setItemDelegateForColumn(0, m_notEditable);

	QVBoxLayout *buttonsLayout = new QVBoxLayout();
	m_pbUp->setEnabled(false);
	m_pbDown->setEnabled(false);
	m_pbRemove->setEnabled(false);
	buttonsLayout->addWidget(m_pbUp);
	buttonsLayout->addWidget(m_pbDown);
	buttonsLayout->addWidget(m_pbAdd);
	buttonsLayout->addWidget(m_pbRemove);
	buttonsLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addLayout(buttonsLayout);
	mainLayout->addWidget(m_driversTree);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(mainLayout);

	for (QVector<QSharedPointer<StepperDriver> >::const_iterator i = drivers.begin(); i != drivers.end(); ++i) {
		addStepperDriver(*i);
	}

	connect(m_pbUp, SIGNAL(clicked()), this, SLOT(on_pbUp_clicked()));
	connect(m_pbDown, SIGNAL(clicked()), this, SLOT(on_pbDown_clicked()));
	connect(m_pbAdd, SIGNAL(clicked()), this, SLOT(on_pbAdd_clicked()));
	connect(m_pbRemove, SIGNAL(clicked()), this, SLOT(on_pbRemove_clicked()));

	connect(m_driversTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(on_driversTree_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

StepperDriversWdg::~StepperDriversWdg()
{
	delete m_notEditable;
}

QVector<QSharedPointer<StepperDriver> > StepperDriversWdg::stepperDrivers() const
{
	QVector<QSharedPointer<StepperDriver> > drivers;

	for (int i = 0; i < m_driversTree->topLevelItemCount(); ++i) {
		StepperDriverTreeItem *driverI = static_cast<StepperDriverTreeItem *>(m_driversTree->topLevelItem(i));
		if (!driverI) {
			continue;
		}
		drivers.push_back(driverI->driver());
	}

	return drivers;
}

QTreeWidgetItem *StepperDriversWdg::currentItem() const
{
	return m_driversTree->currentItem();
}

void StepperDriversWdg::on_driversTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if (current->parent()) {
		m_pbUp->setEnabled(false);
		m_pbDown->setEnabled(false);
		m_pbRemove->setEnabled(false);
	} else {
		m_pbRemove->setEnabled(true);

		int currItemI = m_driversTree->indexOfTopLevelItem(current);
		m_pbUp->setEnabled(currItemI != 0);
		m_pbDown->setEnabled(currItemI < m_driversTree->topLevelItemCount() - 1);
	}

	emit currentItemChanged(current, previous);
}

void StepperDriversWdg::on_pbUp_clicked()
{
	int currItemI = m_driversTree->indexOfTopLevelItem(m_driversTree->currentItem());

	if (currItemI <= 0) {
		return;
	}

	QTreeWidgetItem *currItem = m_driversTree->takeTopLevelItem(currItemI);

	if (!currItem) {
		return;
	}

	m_driversTree->insertTopLevelItem(currItemI - 1 , currItem);
	m_driversTree->setCurrentItem(currItem);
}

void StepperDriversWdg::on_pbDown_clicked()
{
	int currItemI = m_driversTree->indexOfTopLevelItem(m_driversTree->currentItem());

	if (currItemI < 0 || currItemI >= m_driversTree->topLevelItemCount() - 1) {
		return;
	}

	QTreeWidgetItem *currItem = m_driversTree->takeTopLevelItem(currItemI);

	if (!currItem) {
		return;
	}

	m_driversTree->insertTopLevelItem(currItemI + 1 , currItem);
	m_driversTree->setCurrentItem(currItem);
}

void StepperDriversWdg::on_pbAdd_clicked()
{
	addStepperDriver(QSharedPointer<StepperDriver>(new StepperDriver()));
}

void StepperDriversWdg::on_pbRemove_clicked()
{
	StepperDriverTreeItem *drvItem = static_cast<StepperDriverTreeItem *>(m_driversTree->currentItem());
	if (drvItem->propType() != StepperDriverTreeItem::root) {
		return;
	}

	delete m_driversTree->takeTopLevelItem(m_driversTree->indexOfTopLevelItem(m_driversTree->currentItem()));
}

void StepperDriversWdg::addStepperDriver(QSharedPointer<StepperDriver> driver)
{
	if (!driver) {
		return;
	}

	m_driversTree->addTopLevelItem(new StepperDriverTreeItem(driver));
}
