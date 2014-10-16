#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

extern "C" {
#include "gtkwave/debug.h"	// TimeType.
}

#include <QString>
#include <QSharedPointer>
#include <QMetaType>

class Signal;
class PrinterState;

class StepperDriver
{
	Q_DISABLE_COPY(StepperDriver)

public:
	enum SignalType {Inval, Enable, Dir, Step};

	StepperDriver();
	~StepperDriver();

	void setName(QString name);
	QString name() const;

	void setStepsPerMM(double stepsPerMM);
	double stepsPerMM() const;

	void bindToPrinterState(PrinterState *printerState, int vectorIndex);
	void unbindPrinterState() {
		m_printerState = 0;
		m_stateVectorIndex = 0;
	}

	void bindSignal(SignalType sigType, Signal *sig);
	void unbindSignal(Signal *sig);
	Signal *getSignal(SignalType sigType) const;

	void setSignalPolarity(SignalType sigType, bool positive);
	bool signalPolarity(SignalType sigType) const;

	void sigChanged(Signal *sig, TimeType time);

	static QString signalName(SignalType sigType);

private:
	QString m_name;
	double m_stepsPerMM;
	double m_stepLength;

	Signal *m_dirSig;
	Signal *m_stepSig;
	PrinterState *m_printerState;
	int m_stateVectorIndex;
};

Q_DECLARE_METATYPE(QSharedPointer<StepperDriver>)

#include "Signal.h"
#include "PrinterState.h"

inline void StepperDriver::sigChanged(Signal *sig, TimeType time)
{
	if (!m_printerState) {
		return;
	}

	if (sig == m_stepSig && sig->getValue()) {
		// Positive edge.
		if (m_dirSig) {
			if (m_dirSig->getValue()) {
				m_printerState->position[m_stateVectorIndex] -= m_stepLength;
			} else {
				m_printerState->position[m_stateVectorIndex] += m_stepLength;
			}
			m_printerState->timestamp = time;
		}
	}
}

#endif // STEPPER_DRIVER_H
