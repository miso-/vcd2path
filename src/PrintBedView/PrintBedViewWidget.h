#ifndef PRINTBEDVIEWWIDGET_H
#define PRINTBEDVIEWWIDGET_H

#include <QWidget>

class PrintBedView;
class Vcd2path;
class QComboBox;
class QPushButton;
class QRadioButton;

class PrintBedViewWidget : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(PrintBedViewWidget)

public:
	explicit PrintBedViewWidget(Vcd2path *vcd2path, QWidget *parent = 0);
	virtual ~PrintBedViewWidget();

public slots:
	void reloadData();
	void on_pbPlot_clicked();
	void on_gridRBtn_toggled();

private:
	void drawData();

	PrintBedView *m_printBedView;
	QComboBox *m_cbX;
	QComboBox *m_cbY;
	QPushButton *m_pbPlot;
	Vcd2path *m_vcd2path;

	QRadioButton *m_offRBtn, *m_foregroundRBtn, *m_backgroundRBtn;
};

#endif // PRINTBEDVIEWWIDGET_H
