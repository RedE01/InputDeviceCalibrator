#include "X11Calibrator.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput.h>

#include <cstring>
#include <iostream>

std::optional<XID> getDeviceID(const char* deviceName, Display* display) {
	int ndevices;
	XDeviceInfo* devices = XListInputDevices(display, &ndevices);
	for(int i = 0; i < ndevices; ++i) {
		if(std::strcmp(devices[i].name, deviceName) == 0) {
			return devices[i].id;
		}
	}
	return {};
}

X11Calibrator::X11Calibrator(const char* deviceName, ScreenInfo screenInfo) : Calibrator(deviceName, screenInfo) {

}

bool X11Calibrator::deviceExists() const {
	Display* display = XOpenDisplay(0);
	std::optional<XID> deviceID = getDeviceID(getDeviceName(), display);
	XCloseDisplay(display);

	return deviceID.has_value();
}

std::optional<CalibrationData> X11Calibrator::getOldCalibrationData() const {
	Display* display = XOpenDisplay(0);

	std::optional<XID> deviceID = getDeviceID(getDeviceName(), display);
	if(!deviceID.has_value()) {
		XCloseDisplay(display);
		return {};
	}

	Atom property = XInternAtom(display, "Coordinate Transformation Matrix", True);
	if(!property) return {};

	XDevice* inputDevice = XOpenDevice(display, deviceID.value());

	Atom actualType;
	int actualFormat;
	unsigned long nitems;
	unsigned long bytesAfter;
	unsigned char* data;
	XGetDeviceProperty(display, inputDevice, property, 0, 9, False, AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &data);
	XCloseDevice(display, inputDevice);

	CalibrationData oldCalibrationData;
	oldCalibrationData.xScale = *((float*)(data + 0));
	oldCalibrationData.xOffset = *((float*)(data + 16));
	oldCalibrationData.yScale = *((float*)(data + 32));
	oldCalibrationData.yOffset = *((float*)(data + 40));
	
	return oldCalibrationData;
}