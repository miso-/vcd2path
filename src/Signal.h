#ifndef SIGNAL_H
#define SIGNAL_H

#include "gtkwave/debug.h"

#include <QVector>
#include <QString>

class StepperDriver;

class Signal
{
	Q_DISABLE_COPY(Signal)

	friend class StepperDriver;

public:
	Signal(int facNo);
	Signal(const QString &name);
	~Signal();

	int facNo() const {
		return m_facNo;
	}
	QString name() const {
		return m_name;
	}
	void changeValue(uchar value, TimeType time);


	uchar getValue() const {
		return m_value;
	}

private:
	void addLinkTo(StepperDriver *driver);
	void removeLinkTo(StepperDriver *driver);

	int m_facNo;
	QString m_name;
	uchar m_value;
	QVector<StepperDriver *> m_drivers;
};

#include "StepperDriver.h"

inline void Signal::changeValue(uchar value, TimeType time)
{
	m_value = value;
	foreach (StepperDriver * driver, m_drivers) {
		driver->sigChanged(this, time);
	}
}

#endif // SIGNAL_H
