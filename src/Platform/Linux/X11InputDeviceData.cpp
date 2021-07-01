#ifdef IDC_LINUX
#include "../../InputDeviceData.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput.h>
#include <optional>
#include <cstring>

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

InputDeviceData::InputDeviceData(const char* inputDeviceName) : m_name(inputDeviceName) {
	Display* display = XOpenDisplay(0);

	std::optional<XID> deviceID = getDeviceID(inputDeviceName, display);
	if(!deviceID.has_value()) {
		XCloseDisplay(display);
		return;
	}

	Atom property = XInternAtom(display, "Coordinate Transformation Matrix", True);
	if(!property) {
		XCloseDisplay(display);
		return;
	}

	XDevice* inputDevice = XOpenDevice(display, deviceID.value());

	Atom actualType;
	int actualFormat;
	unsigned long nitems;
	unsigned long bytesAfter;
	unsigned char* data;
	XGetDeviceProperty(display, inputDevice, property, 0, 9, False, AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &data);
	XCloseDevice(display, inputDevice);

	for(int i = 0; i < 9; ++i) {
		coordinateTransformMatrix[i] = *((float*)(data + 8 * i));
	}

	m_deviceExists = true;
}
#endif