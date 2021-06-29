#include "CalibratorApplication.h"
#include "ScreenInfo.h"
#include "Calibrator.h"

#include <iostream>

CalibratorApplication::CalibratorApplication(const char* monitorName) : m_monitorName(monitorName) {

}

CalibratorApplication::~CalibratorApplication() {

}

void CalibratorApplication::onInit() {
}

void CalibratorApplication::onUpdate() {
}

void CalibratorApplication::onEvent(rgl::Event* event) {
	if(event->GetEventType() == rgl::EventType::KeyPressed && ((rgl::KeyPressedEvent*)event)->key == rgl::Key::Esc) {
		window->windowOpen = false;
	}
	if(event->GetEventType() == rgl::EventType::WindowExpose) redraw();

	if(event->GetEventType() == rgl::EventType::MouseButtonPressed && ((rgl::MouseButtonPressedEvent*)event)->button == rgl::MouseButton::LEFT) {
		std::cout << ((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.x << ", " << ((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.y << std::endl;
	}
}

void CalibratorApplication::redraw() {
	window->clearScreen();
}