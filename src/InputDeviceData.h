#pragma once
#include <string>

class InputDeviceData {
public:
	InputDeviceData(const char* inputDeviceName);

	bool deviceExists() const { return m_deviceExists; }
	const char* getName() const { return m_name.c_str(); }
	std::string getCoordinateTransformMatrixString() const;

public:
	float coordinateTransformMatrix[9];
	
private:
	const std::string m_name;
	bool m_deviceExists = false;
};