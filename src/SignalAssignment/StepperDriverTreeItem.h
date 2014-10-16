#ifndef STEPPERDRIVERTREEITEM_H
#define STEPPERDRIVERTREEITEM_H

#include "../StepperDriver.h"

#include <QTreeWidgetItem>
#include <QSharedPointer>
#include <QVariant>

class StepperDriverTreeItem : public QTreeWidgetItem
{
public:
	enum AxisFields {root, name, stepsPerMM, sigName, sigPol};

	StepperDriverTreeItem(QSharedPointer<StepperDriver> driver);

	virtual void setData(int column, int role, const QVariant &value);
	virtual QVariant data(int column, int role) const;
	AxisFields propType() const;
	QSharedPointer<StepperDriver> driver() const;

private:
	StepperDriverTreeItem(QSharedPointer<StepperDriver> driver, AxisFields propType,
						  StepperDriver::SignalType sigType = StepperDriver::Inval);

};

#endif // STEPPERDRIVERTREEITEM_H
