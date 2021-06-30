#pragma once
#include <memory>

struct CalibrationData {
	float xScale;
	float xOffset;
	float yScale;
	float yOffset;
};

class Calibrator {
protected:
	Calibrator(const char* deviceName);

public:
	static std::unique_ptr<Calibrator> Create(const char* deviceName);
};