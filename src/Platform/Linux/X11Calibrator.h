#pragma once
#include "../../Calibrator.h"

class X11Calibrator : public Calibrator {
public:
	X11Calibrator(const char* deviceName, ScreenInfo screenInfo);
	virtual bool deviceExists() const override;

protected:
	virtual std::optional<CalibrationData> getOldCalibrationData() const override;
};