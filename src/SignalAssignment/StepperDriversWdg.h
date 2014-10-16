#ifndef STEPPERDRIVERSWDG_H
#define STEPPERDRIVERSWDG_H

#include "../StepperDriver.h"

#include <QWidget>
#include <QSharedPointer>

class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class NotEditable;

class StepperDriversWdg : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(StepperDriversWdg)

public:
	enum AxisFields {inval, name, stepsPerMM, sigName, sigPol};

	explicit StepperDriversWdg(const QVector<QSharedPointer<StepperDriver> > &drivers, QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~StepperDriversWdg();

	QVector<QSharedPointer<StepperDriver> > stepperDrivers() const;
	QTreeWidgetItem *currentItem() const;

	static bool isSigReceptor(const QTreeWidgetItem &item);

private slots:
	void on_driversTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_pbUp_clicked();
	void on_pbDown_clicked();
	void on_pbAdd_clicked();
	void on_pbRemove_clicked();

signals:
	void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
	void addStepperDriver(QSharedPointer<StepperDriver> driver);
	void addSignalProps(QTreeWidgetItem *driverItem, const QString &name, StepperDriver::SignalType sigType);

	QPushButton *m_pbUp, *m_pbDown, *m_pbAdd, *m_pbRemove;
	QTreeWidget *m_driversTree;
	NotEditable *m_notEditable;
};

#endif // STEPPERDRIVERSWDG_H
