#pragma once
#include "../../Calibrator.h"

class X11Calibrator : public Calibrator {
public:
	X11Calibrator(const char* deviceName);

private:
	CalibrationData oldCalibrationData;
};