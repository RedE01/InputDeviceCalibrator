#ifdef IDC_LINUX
#include "../../ScreenInfo.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>
#include <cstring>

ScreenInfo::ScreenInfo(const char* screenName) : m_name(screenName), m_screenFound(false) {
	unsigned int screenNameLength = std::strlen(screenName);

	Display* display = XOpenDisplay(0);
	Window rootWindow = XDefaultRootWindow(display);

	XRRScreenResources* screenResources = XRRGetScreenResources(display, rootWindow);

	for(int i = 0; i < screenResources->ncrtc; ++i) {
		XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(display, screenResources, screenResources->crtcs[i]);

		if(crtcInfo->mode != None && crtcInfo->noutput > 0) {
			XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenResources, crtcInfo->outputs[0]);
			if(std::strcmp(screenName, outputInfo->name) == 0 || screenNameLength == 0) {
				m_screenFound = true;
				m_x = crtcInfo->x;
				m_y = crtcInfo->y;
				m_width = crtcInfo->width;
				m_height = crtcInfo->height;
				break;
			}
		}
	}

	XRRScreenConfiguration* screenInfo = XRRGetScreenInfo(display, rootWindow);
	int screen = XDefaultScreen(display);
	m_rootScreenWidth = XDisplayWidth(display, screen);
	m_rootScreenHeight = XDisplayHeight(display, screen);

	XCloseDisplay(display);
}
#endif