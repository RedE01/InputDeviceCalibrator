#pragma once
#include "ScreenInfo.h"
#include "Vector2.h"
#include <memory>
#include <optional>
#include <string>

struct CalibrationData {
	float xScale;
	float xOffset;
	float yScale;
	float yOffset;
};

class Calibrator {
public:
	static std::unique_ptr<Calibrator> Create(const char* deviceName, ScreenInfo screenInfo);

	virtual bool deviceExists() const = 0;
	void registerClick(int clickX, int clickY);
	std::optional<CalibrationData> calculateCalibrationData() const;

	int getNumberOfClicks() const { return m_numberOfClicks; }
	const char* getDeviceName() const { return m_deviceName.c_str(); }
	Vector2<int> getTargetScreenCoordinates(unsigned int targetNumber);

	static std::string GetCoordinateTransformMatrixString(CalibrationData calibrationData);

protected:
	Calibrator(const char* deviceName, ScreenInfo screenInfo);

	virtual std::optional<CalibrationData> getOldCalibrationData() const = 0;

private:
	const std::string m_deviceName;
	ScreenInfo m_screenInfo;
	Vector2<float> m_clicks[4];
	int m_numberOfClicks;
	Vector2<float> m_targets[4];
};