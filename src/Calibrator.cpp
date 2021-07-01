#include "Calibrator.h"

Calibrator::Calibrator(InputDeviceData inputDeviceData, ScreenInfo screenInfo) : m_oldInputDeviceData(inputDeviceData), m_screenInfo(screenInfo), m_numberOfClicks(0) {
	Vector2<float> screenPosNormalized(screenInfo.getX() / float(screenInfo.getRootScreenWidth()), screenInfo.getY() / float(screenInfo.getRootScreenHeight()));
	Vector2<float> screenSizeNormalized(screenInfo.getWidth() / float(screenInfo.getRootScreenWidth()), screenInfo.getHeight() / float(screenInfo.getRootScreenHeight()));
	float targetOffsetScaler = 0.1;
	Vector2<float> targetOffsetNormalized(screenSizeNormalized.x * targetOffsetScaler, screenSizeNormalized.y * targetOffsetScaler);

	m_targets[0].x = screenPosNormalized.x + targetOffsetNormalized.x;
	m_targets[0].y = screenPosNormalized.y + targetOffsetNormalized.y;

	m_targets[1].x = screenPosNormalized.x + screenSizeNormalized.x - targetOffsetNormalized.x;
	m_targets[1].y = screenPosNormalized.y + targetOffsetNormalized.y;

	m_targets[2].x = screenPosNormalized.x + targetOffsetNormalized.x;
	m_targets[2].y = screenPosNormalized.y + screenSizeNormalized.y - targetOffsetNormalized.y;

	m_targets[3].x = screenPosNormalized.x + screenSizeNormalized.x - targetOffsetNormalized.x;
	m_targets[3].y = screenPosNormalized.y + screenSizeNormalized.y - targetOffsetNormalized.y;
}

void Calibrator::registerClick(int clickX, int clickY) {
	if(m_numberOfClicks >= 4) return;

	m_clicks[m_numberOfClicks].x = clickX / float(m_screenInfo.getRootScreenWidth());
	m_clicks[m_numberOfClicks].y = clickY / float(m_screenInfo.getRootScreenHeight());
	m_numberOfClicks++;
}

std::optional<InputDeviceData> Calibrator::calculateCalibrationData() const {
	if(m_numberOfClicks < 4) return {};

	Vector2<float> rawClicks[4];
	for(int i = 0; i < 4; ++i) {
		rawClicks[i].x = (m_clicks[i].x - m_oldInputDeviceData.coordinateTransformMatrix[2]) / m_oldInputDeviceData.coordinateTransformMatrix[0];
		rawClicks[i].y = (m_clicks[i].y - m_oldInputDeviceData.coordinateTransformMatrix[5]) / m_oldInputDeviceData.coordinateTransformMatrix[4];
	}

	Vector2<float> rawClick1((rawClicks[0].x + rawClicks[2].x) / 2.0, (rawClicks[0].y + rawClicks[1].y) / 2.0);
	Vector2<float> rawClick2((rawClicks[1].x + rawClicks[3].x) / 2.0, (rawClicks[2].y + rawClicks[3].y) / 2.0);

	Vector2<float> target1 = m_targets[0];
	Vector2<float> target2 = m_targets[3];

	InputDeviceData calibrationData(m_oldInputDeviceData);
	calibrationData.coordinateTransformMatrix[0] = (target2.x - target1.x) / (rawClick2.x - rawClick1.x);
	calibrationData.coordinateTransformMatrix[4] = (target2.y - target1.y) / (rawClick2.y - rawClick1.y);
	calibrationData.coordinateTransformMatrix[2] = target1.x - rawClick1.x * calibrationData.coordinateTransformMatrix[0];
	calibrationData.coordinateTransformMatrix[5] = target1.y - rawClick1.y * calibrationData.coordinateTransformMatrix[4];

	return calibrationData;
}

Vector2<int> Calibrator::getTargetScreenCoordinates(unsigned int targetNumber) {
	if(targetNumber >= 4) return Vector2<int>();

	Vector2<int> target(m_targets[targetNumber].x * m_screenInfo.getRootScreenWidth(), m_targets[targetNumber].y * m_screenInfo.getRootScreenHeight());
	Vector2<int> targetLocal(target.x - m_screenInfo.getX(), target.y - m_screenInfo.getY());
	return targetLocal;
}