#include "Signal.h"

extern "C" {
#include "gtkwave/globals.h"
}

Signal::Signal(int facNo)
	: m_facNo(facNo),
	  m_name(),
	  m_value(),
	  m_drivers()
{
	if (m_facNo >= 0 && m_facNo < GLOBALS->numfacs) {
		m_name = QString::fromAscii(GLOBALS->facs[m_facNo]->name);
	}
}

Signal::Signal(const QString &name)
	: m_facNo(-1),
	  m_name(name),
	  m_value(),
	  m_drivers()
{

}

Signal::~Signal()
{
	QVector<StepperDriver *> drivers = m_drivers;

	for (QVector<StepperDriver *>::iterator driverI = drivers.begin(); driverI != drivers.end(); ++driverI) {
		if (*driverI) {
			(*driverI)->unbindSignal(this);
		}
	}
}

void Signal::addLinkTo(StepperDriver *driver)
{
	if (m_drivers.indexOf(driver) == -1) {
		m_drivers.push_back(driver);
	}
}

void Signal::removeLinkTo(StepperDriver *driver)
{
	if (m_drivers.indexOf(driver) != -1) {
		m_drivers.remove(m_drivers.indexOf(driver));
	}
}
