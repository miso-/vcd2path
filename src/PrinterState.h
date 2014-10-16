#ifndef PRINTERSTATE_H
#define PRINTERSTATE_H

extern "C" {
#include "gtkwave/debug.h"	// TimeType.
}

#include <QVector>

struct PrinterState {
	PrinterState() : position(), timestamp(-1)
	{}

	QVector<double> position;
	TimeType timestamp;
};

#endif // PRINTERSTATE_H
