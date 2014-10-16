#ifndef Vcd2path_H
#define Vcd2path_H

#include "PrinterState.h"

#include <QtGui/QMainWindow>
#include <QVector>

class PrintBedViewWidget;
class SignalAssignmentDia;
class Signal;
class StepperDriver;
class QArray;

namespace Ui
{
class Vcd2path;
}

class Vcd2path : public QMainWindow
{
	Q_OBJECT
	Q_DISABLE_COPY(Vcd2path)

public:
	Vcd2path();
	virtual ~Vcd2path();

	const QVector<QSharedPointer<StepperDriver> > &drivers() const;
	const QVector<PrinterState> &stateTrace() const;

private slots:
	void on_action_Open_triggered();
	void on_action_Quit_triggered();
	void on_action_About_triggered();

private:
	void createTrace();

	Ui::Vcd2path *ui;
	PrintBedViewWidget *m_printBedView;

	QVector<QSharedPointer<Signal> > m_sigs;
	QVector<QSharedPointer<StepperDriver> > m_drivers;
	PrinterState *m_currState;
	QVector<PrinterState> m_stateTrace;
};

#endif // Vcd2path_H
