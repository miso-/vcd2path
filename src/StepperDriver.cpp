#include "StepperDriver.h"

#include "Signal.h"
#include "PrinterState.h"

#include <limits>

StepperDriver::StepperDriver()
	: m_name("New axis"),
	  m_stepsPerMM(0),
	  m_stepLength(0),
	  m_dirSig(0),
	  m_stepSig(0),
	  m_printerState(0),
	  m_stateVectorIndex(0)
{

}

StepperDriver::~StepperDriver()
{
	if (m_dirSig) {
		m_dirSig->removeLinkTo(this);
	}

	if (m_stepSig) {
		m_stepSig->removeLinkTo(this);
	}
}

void StepperDriver::setName(QString name)
{
	m_name = name;
}

QString StepperDriver::name() const
{
	return m_name;
}

void StepperDriver::setStepsPerMM(double stepsPerMM)
{
	m_stepsPerMM = stepsPerMM;

	if (m_stepsPerMM == 0.0) {
		m_stepLength = 0.0;
	} else {
		m_stepLength = 1.0 / m_stepsPerMM;
	}
}

double StepperDriver::stepsPerMM() const
{
	return m_stepsPerMM;
}

void StepperDriver::bindToPrinterState(PrinterState *printerState, int vectorIndex)
{
	if (!printerState || vectorIndex < 0 || vectorIndex == std::numeric_limits<int>::max()) {
		// ERROR.
		return;
	}

	m_stateVectorIndex = vectorIndex;

	if (printerState->position.size() <= vectorIndex) {
		printerState->position.resize(vectorIndex + 1);
	}

	m_printerState = printerState;
}

void StepperDriver::bindSignal(StepperDriver::SignalType sigType, Signal *sig)
{
	Signal **sigTarget = 0;

	switch (sigType) {

	case Dir:
		sigTarget = &m_dirSig;
		break;

	case Step:
		sigTarget = &m_stepSig;
		break;

	default:
		sigTarget = 0;
	}

	if (!sigTarget) {
		return;
	}

	if (*sigTarget) {
		(*sigTarget)->removeLinkTo(this);
		*sigTarget = 0;
	}

	if (sig) {
		sig->addLinkTo(this);
	}

	*sigTarget = sig;
}

void StepperDriver::unbindSignal(Signal *sig)
{
	bool wasBound = false;

	if (m_dirSig == sig) {
		m_dirSig = 0;
		wasBound = true;
	}

	if (m_stepSig == sig) {
		m_stepSig = 0;
		wasBound = true;
	}

	if (wasBound) {
		sig->removeLinkTo(this);
	}
}

Signal *StepperDriver::getSignal(StepperDriver::SignalType sigType) const
{
	switch (sigType) {

	case Dir:
		return m_dirSig;
		break;

	case Step:
		return m_stepSig;
		break;

	default:
		return 0;
	}
}

void StepperDriver::setSignalPolarity(StepperDriver::SignalType sigType, bool positive)
{
	// TODO:
}

bool StepperDriver::signalPolarity(StepperDriver::SignalType sigType) const
{
	// TODO:

	return true;
}

QString StepperDriver::signalName(StepperDriver::SignalType sigType)
{
	switch (sigType) {
	case Enable:
		return "Enable";
		break;

	case Dir:
		return "Dir";
		break;

	case Step:
		return "Step";
		break;

	default:
		return "INVALID";
	}
}
