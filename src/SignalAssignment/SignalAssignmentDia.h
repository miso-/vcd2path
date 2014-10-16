#ifndef SIGNALASSIGNMENTDIA_H
#define SIGNALASSIGNMENTDIA_H

#include "../StepperDriver.h"

#include <QDialog>
#include <QVector>
#include <QSharedPointer>

class Signal;
class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;
class QSettings;
class StepperDriversWdg;

class SignalAssignmentDia : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY(SignalAssignmentDia)

public:
	SignalAssignmentDia(QWidget *parent = 0, Qt::WindowFlags f = 0);

	QVector<QSharedPointer<StepperDriver> > stepperDrivers() const;
	QVector<QSharedPointer<Signal> > getSignals() const;

private slots:
	void on_driversTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_sigTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_pbOk_clicked();
	void on_pbSave_clicked();
	void on_pbCancel_clicked();
	void on_pbConnect_clicked();

private:
	QSharedPointer<Signal> getSignalByName(const QString &name);
	void loadDrivers();
	void loadDriverSigProps(const QSettings &settings,  QSharedPointer<StepperDriver> driver, StepperDriver::SignalType sigtype);
	void saveDrivers() const;
	void saveDriverSigProps(QSettings &settings, QSharedPointer<StepperDriver> driver, StepperDriver::SignalType sigtype) const;

	void addSigToTree(struct tree *t, QTreeWidgetItem *where = 0);

	StepperDriversWdg *m_driversTree;
	QTreeWidget *m_sigTree;
	QPushButton *m_pbOk, *m_pbSave, *m_pbCancel, *m_pbConnect;

	QVector<QSharedPointer<Signal> > m_sigs;
	QVector<QSharedPointer<StepperDriver> > m_drivers;
};

#endif // SIGNALASSIGNMENTDIA_H
