#pragma once
#include "ScreenInfo.h"
#include "InputDeviceData.h"
#include "Vector2.h"
#include <optional>

class Calibrator {
public:
	Calibrator(InputDeviceData inputDeviceData, ScreenInfo screenInfo);

	void registerClick(int clickX, int clickY);
	std::optional<InputDeviceData> calculateCalibrationData() const;

	int getNumberOfClicks() const { return m_numberOfClicks; }
	const char* getDeviceName() const { return m_oldInputDeviceData.getName(); }
	Vector2<int> getTargetScreenCoordinates(unsigned int targetNumber);

private:
	const ScreenInfo m_screenInfo;
	const InputDeviceData m_oldInputDeviceData;
	int m_numberOfClicks;
	Vector2<float> m_clicks[4];
	Vector2<float> m_targets[4];
};