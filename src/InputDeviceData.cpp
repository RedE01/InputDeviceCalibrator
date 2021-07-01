#include "InputDeviceData.h"
#include <sstream>

std::string InputDeviceData::getCoordinateTransformMatrixString() const {
	std::stringstream ss;
	for(int i = 0; i < 9; ++i) {
		ss << coordinateTransformMatrix[i] << " ";
	}
	return ss.str();
}