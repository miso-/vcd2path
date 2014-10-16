#include "SignalAssignmentDia.h"

#include "../StepperDriver.h"
#include "../Signal.h"
#include "StepperDriversWdg.h"
#include "StepperDriverTreeItem.h"

extern "C" {
#include "../gtkwave/tree.h"
#include "../gtkwave/globals.h"
}

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QSettings>

SignalAssignmentDia::SignalAssignmentDia(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f),
	  m_driversTree(0),
	  m_sigTree(new QTreeWidget()),
	  m_pbOk(new QPushButton("Ok")),
	  m_pbSave(new QPushButton("Save")),
	  m_pbCancel(new QPushButton("Cancel")),
	  m_pbConnect(new QPushButton("<->")),
	  m_sigs(), m_drivers()
{
	setWindowTitle("Signal assignment");

	addSigToTree(GLOBALS->treeroot);
	loadDrivers();


	m_sigTree->setHeaderLabel("Signals:");

	m_driversTree = new StepperDriversWdg(m_drivers, this);

	m_pbConnect->setEnabled(false);
	QHBoxLayout *treesLayout = new QHBoxLayout();
	treesLayout->addWidget(m_driversTree);
	treesLayout->addWidget(m_pbConnect);
	treesLayout->addWidget(m_sigTree);

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttonsLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttonsLayout->addWidget(m_pbOk);
	buttonsLayout->addWidget(m_pbSave);
	buttonsLayout->addWidget(m_pbCancel);

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addLayout(treesLayout);
	mainLayout->addLayout(buttonsLayout);

	this->setLayout(mainLayout);

	connect(m_pbOk, SIGNAL(clicked()), this, SLOT(on_pbOk_clicked()));
	connect(m_pbSave, SIGNAL(clicked()), this, SLOT(on_pbSave_clicked()));
	connect(m_pbCancel, SIGNAL(clicked()), this, SLOT(on_pbCancel_clicked()));
	connect(m_pbConnect, SIGNAL(clicked()), this, SLOT(on_pbConnect_clicked()));

	connect(m_driversTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(on_driversTree_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
	connect(m_sigTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
			this, SLOT(on_sigTree_currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
}

QVector<QSharedPointer<StepperDriver> > SignalAssignmentDia::stepperDrivers() const
{
	return m_driversTree->stepperDrivers();
}

QVector<QSharedPointer<Signal> > SignalAssignmentDia::getSignals() const
{
	return m_sigs;
}

void SignalAssignmentDia::on_driversTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	Q_UNUSED(current)
	Q_UNUSED(previous)

	StepperDriverTreeItem *currDriver = static_cast<StepperDriverTreeItem *>(m_driversTree->currentItem());
	QTreeWidgetItem *currSig = m_sigTree->currentItem();

	m_pbConnect->setEnabled(currDriver && currDriver->propType() == StepperDriverTreeItem::sigName &&
							currSig && currSig->parent());
}

void SignalAssignmentDia::on_sigTree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	Q_UNUSED(current)
	Q_UNUSED(previous)

	StepperDriverTreeItem *currDriver = static_cast<StepperDriverTreeItem *>(m_driversTree->currentItem());
	QTreeWidgetItem *currSig = m_sigTree->currentItem();

	m_pbConnect->setEnabled(currDriver && currDriver->propType() == StepperDriverTreeItem::sigName &&
							currSig && currSig->parent());
}

void SignalAssignmentDia::on_pbOk_clicked()
{
	accept();
}

void SignalAssignmentDia::on_pbSave_clicked()
{
	saveDrivers();
}

void SignalAssignmentDia::on_pbCancel_clicked()
{
	reject();
}

void SignalAssignmentDia::on_pbConnect_clicked()
{
	StepperDriverTreeItem *drvItem = static_cast<StepperDriverTreeItem *>(m_driversTree->currentItem());

	Signal *sig = 0;
	if (m_sigTree->currentItem()->data(0, Qt::UserRole).value<void *>()) {
		sig = static_cast<Signal *>(m_sigTree->currentItem()->data(0, Qt::UserRole).value<void *>());
	}

	if (drvItem->propType() == StepperDriverTreeItem::sigName && sig) {
		drvItem->setData(1, Qt::DisplayRole, QVariant::fromValue<void *>(sig));
	}
}

QSharedPointer<Signal> SignalAssignmentDia::getSignalByName(const QString &name)
{
	// Try to find signal with given name.
	foreach (QSharedPointer<Signal> sig, m_sigs) {
		if (sig->name() == name) {
			return sig;
		}
	}

	// If signal does not exist, create new one.
	QSharedPointer<Signal> sig(new Signal(name));
	m_sigs.push_back(sig);
	return sig;
}

void SignalAssignmentDia::loadDrivers()
{
	QSettings settings;

	int numAxes = settings.beginReadArray("axes");
	for (int i = 0; i < numAxes; ++i) {
		QSharedPointer<StepperDriver> driver(new StepperDriver());
		settings.setArrayIndex(i);
		driver->setName(settings.value("name").toString());
		driver->setStepsPerMM(settings.value("steps_per_mm").toUInt());
		loadDriverSigProps(settings, driver, StepperDriver::Dir);
		loadDriverSigProps(settings, driver, StepperDriver::Step);
		m_drivers.push_back(driver);
	}
}

void SignalAssignmentDia::loadDriverSigProps(const QSettings &settings, QSharedPointer<StepperDriver> driver, StepperDriver::SignalType sigtype)
{
	if (!driver) {
		return;
	}

	QString sigName = settings.value(QString::number(sigtype)).toString();
	QSharedPointer<Signal> sig = getSignalByName(sigName);
	if (sig) {
		driver->bindSignal(sigtype, sig.data());
	}
}

void SignalAssignmentDia::saveDrivers() const
{
	QSettings settings;
	QVector<QSharedPointer<StepperDriver> > drivers = stepperDrivers();

	settings.beginWriteArray("axes");
	for (int i = 0; i < drivers.count(); ++i) {
		settings.setArrayIndex(i);
		settings.setValue("name",  drivers[i]->name());
		settings.setValue("steps_per_mm",  drivers[i]->stepsPerMM());
		saveDriverSigProps(settings, drivers[i], StepperDriver::Dir);
		saveDriverSigProps(settings, drivers[i], StepperDriver::Step);
	}
	settings.endArray();
}

void SignalAssignmentDia::saveDriverSigProps(QSettings &settings, QSharedPointer<StepperDriver> driver, StepperDriver::SignalType sigtype) const
{
	if (!driver) {
		return;
	}

	Signal *sig = driver->getSignal(sigtype);

	QString sigName("");

	if (sig) {
		sigName = sig->name();
	}

	settings.setValue(QString::number(sigtype), sigName);
}

void SignalAssignmentDia::addSigToTree(tree *t, QTreeWidgetItem *where)
{
	QList <QTreeWidgetItem *> treeWidgetItems;

	for (; t != 0; t = t->next) {
		QTreeWidgetItem *twi = new QTreeWidgetItem();
		twi->setText(0, t->name);

		if (t->t_which < 0) {
			Qt::ItemFlags f = twi->flags();
			f &= ~Qt::ItemIsSelectable;
			twi->setFlags(f);
		} else {
			QSharedPointer<Signal> sig(new Signal(t->t_which));
			m_sigs.push_back(sig);
			twi->setData(0, Qt::UserRole, QVariant::fromValue<void *>(sig.data()));
		}

		if (t->child) {
			addSigToTree(t->child, twi);
		}
		treeWidgetItems.push_back(twi);
	}

	if (where) {
		where->addChildren(treeWidgetItems);
	} else {
		m_sigTree->addTopLevelItems(treeWidgetItems);
	}
}
