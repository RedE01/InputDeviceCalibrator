#include "Calibrator.h"
#include "Platform/Linux/X11Calibrator.h"

Calibrator::Calibrator(const char* deviceName) {

}

std::unique_ptr<Calibrator> Calibrator::Create(const char* deviceName) {
	#ifdef IDC_LINUX
		return std::make_unique<X11Calibrator>(deviceName);
	#endif

	return std::unique_ptr<Calibrator>();
}