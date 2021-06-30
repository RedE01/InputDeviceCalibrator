#include "X11Calibrator.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput.h>

#include <cstring>
#include <iostream>

X11Calibrator::X11Calibrator(const char* deviceName) : Calibrator(deviceName) {
	Display* display = XOpenDisplay(0);
	Window rootWindow = XDefaultRootWindow(display);

	int ndevices;
	XDeviceInfo* devices = XListInputDevices(display, &ndevices);
	for(int i = 0; i < ndevices; ++i) {
		if(std::strcmp(devices[i].name, deviceName) == 0) {
			Atom property = XInternAtom(display, "Coordinate Transformation Matrix", True);
			if(!property) break;

			XDevice* inputDevice = XOpenDevice(display, devices[i].id);

			Atom actualType;
			int actualFormat;
			unsigned long nitems;
			unsigned long bytesAfter;
			unsigned char* data;
			XGetDeviceProperty(display, inputDevice, property, 0, 9, False, AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &data);

			oldCalibrationData.xScale = *((float*)(data + 0));
			oldCalibrationData.xOffset = *((float*)(data + 16));
			oldCalibrationData.yScale = *((float*)(data + 32));
			oldCalibrationData.yOffset = *((float*)(data + 40));
			
			XCloseDevice(display, inputDevice);
			break;
		}
	}


	XCloseDisplay(display);
}