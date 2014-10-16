#include "StepperDriverTreeItem.h"

#include "../StepperDriver.h"

#include <QString>

StepperDriverTreeItem::StepperDriverTreeItem(QSharedPointer<StepperDriver> driver)
	: QTreeWidgetItem()
{
	if (driver) {
		QTreeWidgetItem::setData(0, Qt::DisplayRole, driver->name());
	}

	QTreeWidgetItem::setData(0, Qt::UserRole, root);
	QTreeWidgetItem::setData(0, Qt::UserRole + 1, QVariant::fromValue<QSharedPointer<StepperDriver> >(driver));

	addChild(new StepperDriverTreeItem(driver, name));
	addChild(new StepperDriverTreeItem(driver, stepsPerMM));
	addChild(new StepperDriverTreeItem(driver, sigName, StepperDriver::Dir));
	addChild(new StepperDriverTreeItem(driver, sigPol, StepperDriver::Dir));
	addChild(new StepperDriverTreeItem(driver, sigName, StepperDriver::Step));
	addChild(new StepperDriverTreeItem(driver, sigPol, StepperDriver::Step));
}

StepperDriverTreeItem::StepperDriverTreeItem(QSharedPointer<StepperDriver> driver, StepperDriverTreeItem::AxisFields propType,
		StepperDriver::SignalType sigType)
	: QTreeWidgetItem()
{
	QString propName;
	QVariant propVal;
	switch (propType) {
	case name:
		propName = "Name:";
		propVal = driver ? driver->name() : "";
		break;

	case stepsPerMM:
		propName = "Steps per mm:";
		propVal = driver ? driver->stepsPerMM() : 0.0;
		break;

	case sigName:
		propName = StepperDriver::signalName(sigType) + " signal:";
		propVal = driver && driver->getSignal(sigType) ? driver->getSignal(sigType)->name() : "";
		break;

	case sigPol:
		propName = StepperDriver::signalName(sigType) + " signal positive:";
		propVal = driver ? driver->signalPolarity(sigType) : true;
		break;

	default:
		;
	}

	QTreeWidgetItem::setData(0, Qt::DisplayRole, propName);
	QTreeWidgetItem::setData(0, Qt::UserRole, propType);
	QTreeWidgetItem::setData(0, Qt::UserRole + 1, sigType);
	QTreeWidgetItem::setData(1, Qt::DisplayRole, propVal);

	if (propType == name || propType == stepsPerMM || propType == sigPol) {
		setFlags(flags() | Qt::ItemIsEditable);
	}
}

void StepperDriverTreeItem::setData(int column, int role, const QVariant &value)
{
	if (column != 1 || propType() == root) {
		return;
	}

	QVariant val = value;
	QSharedPointer<StepperDriver> drv = driver();

	if ((role == Qt::DisplayRole || role == Qt::EditRole) && drv) {
		switch (propType()) {
		case name:
			drv->setName(value.toString());
			break;

		case stepsPerMM:
			drv->setStepsPerMM(value.toDouble());
			break;

		case sigName:
			drv->bindSignal(static_cast<StepperDriver::SignalType>(data(0, Qt::UserRole + 1).toInt()), static_cast<Signal *>(value.value<void *>()));
			val = value.value<void *>() ? static_cast<Signal *>(value.value<void *>())->name() : "";
			break;

		case sigPol:
			drv->setSignalPolarity(static_cast<StepperDriver::SignalType>(data(0, Qt::UserRole + 1).toInt()), value.toBool());
			break;

		default:
			;
		}
	}

	QTreeWidgetItem::setData(column, role, val);
}

QVariant StepperDriverTreeItem::data(int column, int role) const
{
	return QTreeWidgetItem::data(column, role);
}

StepperDriverTreeItem::AxisFields StepperDriverTreeItem::propType() const
{
	return static_cast<AxisFields>(QTreeWidgetItem::data(0, Qt::UserRole).toInt());
}

QSharedPointer<StepperDriver> StepperDriverTreeItem::driver() const
{
	if (propType() == root) {
		return QTreeWidgetItem::data(0, Qt::UserRole + 1).value<QSharedPointer<StepperDriver> >();
	} else {
		return parent() ? static_cast<StepperDriverTreeItem *>(parent())->driver() : QSharedPointer<StepperDriver>(0);
	}
}
