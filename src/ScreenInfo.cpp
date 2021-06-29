#include "ScreenInfo.h"
#ifdef IDC_LINUX
	#include "Platform/Linux/X11ScreenInfo.h"
#endif

ScreenInfo::ScreenInfo(const char* screenName) : m_name(screenName), m_screenFound(false) {
}

std::unique_ptr<ScreenInfo> ScreenInfo::Create(const char* screenName) {
	#ifdef IDC_LINUX
		return std::make_unique<X11ScreenInfo>(screenName);
	#endif

	return std::unique_ptr<ScreenInfo>();
}