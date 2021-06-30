#include "CalibratorApplication.h"
#include "ScreenInfo.h"
#include "Calibrator.h"

#include <iostream>

CalibratorApplication::CalibratorApplication(const char* inputDeviceName, const char* monitorName) : m_inputDeviceName(inputDeviceName), m_monitorName(monitorName), m_calibrationString("") {

}

CalibratorApplication::~CalibratorApplication() {

}

void CalibratorApplication::onInit() {
	ScreenInfo screenInfo(m_monitorName.c_str());
	if(!screenInfo.screenFound()) {
		std::cout << "Error: Monitor '" << m_monitorName << "' could not be found" << std::endl;
		window->windowOpen = false;
		return;
	}

	m_calibrator = Calibrator::Create(m_inputDeviceName.c_str(), screenInfo);
	if(!m_calibrator->deviceExists()) {
		std::cout << "Error: Input device '" << m_calibrator->getDeviceName() << "' could not be found" << std::endl;
		window->windowOpen = false;
		return;
	}

	rgl::Vector2i monitorPos(screenInfo.getX(), screenInfo.getY());
	rgl::Vector2i monitorSize(screenInfo.getWidth(), screenInfo.getHeight());
	window->setWindowPos(monitorPos);
	window->setWindowSize(monitorSize);
	window->setFullscreen(true);
}

void CalibratorApplication::onUpdate() {
}

void CalibratorApplication::onEvent(rgl::Event* event) {
	if(event->GetEventType() == rgl::EventType::KeyPressed && ((rgl::KeyPressedEvent*)event)->key == rgl::Key::Esc) {
		window->windowOpen = false;
	}
	if(event->GetEventType() == rgl::EventType::WindowExpose) redraw();

	if(event->GetEventType() == rgl::EventType::MouseButtonPressed && ((rgl::MouseButtonPressedEvent*)event)->button == rgl::MouseButton::LEFT) {
		m_calibrator->registerClick(((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.x, ((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.y);

		redraw();

		if(m_calibrator->getNumberOfClicks() == 4) {
			std::optional<CalibrationData> calibrationData = m_calibrator->calculateCalibrationData();

			if(calibrationData.has_value()) m_calibrationString = Calibrator::GetCoordinateTransformMatrixString(calibrationData.value());

			window->windowOpen = false;
		}
	}
}

void CalibratorApplication::redraw() {
	window->clearScreen();
	window->setBackgroundColor(rgl::Vector3(0.8, 0.8, 0.8));

	drawTextBox();

	int targetSize = 64;
	Vector2<int> targetPos = m_calibrator->getTargetScreenCoordinates(m_calibrator->getNumberOfClicks());
	drawTarget(targetPos.x, targetPos.y, targetSize, targetSize);
}

void CalibratorApplication::drawTextBox() {
	window->setDrawColor(rgl::Vector3(0.0, 0.0, 0.0));

	rgl::Vector2i centerPos = rgl::Vector2i(window->getWindowSize().x / 2, window->getWindowSize().y / 2);
	window->drawRectangle(rgl::Vector2i(centerPos.x - 128, centerPos.y - 64), rgl::Vector2i(256, 128));
	window->drawString("Press escape to cancel", rgl::Vector2i(centerPos.x - 64, centerPos.y));
}

void CalibratorApplication::drawTarget(int x, int y, int width, int height) {
	int startX = x - width / 2;
	int startY = y - height / 2;
	int endX = x + width / 2;
	int endY = y + height / 2;
	rgl::Vector2i circleSize = rgl::Vector2i(width * 0.5, height * 0.5);
	rgl::Vector2i circlePos = rgl::Vector2i(x - circleSize.x / 2, y - circleSize.y / 2);

	window->setDrawColor(rgl::Vector3(0.78, 0.78, 0.78));
	window->drawCircleFilled(circlePos, circleSize);

	window->setDrawColor(rgl::Vector3(1.0, 0.0, 0.0));
	window->drawCircle(circlePos, circleSize);
	window->drawLine(rgl::Vector2i(startX, y), rgl::Vector2i(endX, y));
	window->drawLine(rgl::Vector2i(x, startY), rgl::Vector2i(x, endY));
}