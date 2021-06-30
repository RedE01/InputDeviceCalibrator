#include "Calibrator.h"
#include "Platform/Linux/X11Calibrator.h"
#include <sstream>

Calibrator::Calibrator(const char* deviceName, ScreenInfo screenInfo) : m_deviceName(deviceName), m_screenInfo(screenInfo), m_numberOfClicks(0) {
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

std::unique_ptr<Calibrator> Calibrator::Create(const char* deviceName, ScreenInfo screenInfo) {
	#ifdef IDC_LINUX
		return std::make_unique<X11Calibrator>(deviceName, screenInfo);
	#endif

	return std::unique_ptr<Calibrator>();
}

void Calibrator::registerClick(int clickX, int clickY) {
	if(m_numberOfClicks >= 4) return;

	m_clicks[m_numberOfClicks].x = clickX / float(m_screenInfo.getRootScreenWidth());
	m_clicks[m_numberOfClicks].y = clickY / float(m_screenInfo.getRootScreenHeight());
	m_numberOfClicks++;
}

std::optional<CalibrationData> Calibrator::calculateCalibrationData() const {
	if(m_numberOfClicks < 4) return {};

	std::optional<CalibrationData> oldCalibrationData = getOldCalibrationData();
	if(!oldCalibrationData.has_value()) return {};

	Vector2<float> rawClicks[4];
	for(int i = 0; i < 4; ++i) {
		rawClicks[i].x = (m_clicks[i].x - oldCalibrationData->xOffset) / oldCalibrationData->xScale;
		rawClicks[i].y = (m_clicks[i].y - oldCalibrationData->yOffset) / oldCalibrationData->yScale;
	}

	Vector2<float> rawClick1((rawClicks[0].x + rawClicks[2].x) / 2.0, (rawClicks[0].y + rawClicks[1].y) / 2.0);
	Vector2<float> rawClick2((rawClicks[1].x + rawClicks[3].x) / 2.0, (rawClicks[2].y + rawClicks[3].y) / 2.0);

	Vector2<float> target1 = m_targets[0];
	Vector2<float> target2 = m_targets[3];

	CalibrationData calibrationData;
	calibrationData.xScale = (target2.x - target1.x) / (rawClick2.x - rawClick1.x);
	calibrationData.yScale = (target2.y - target1.y) / (rawClick2.y - rawClick1.y);
	calibrationData.xOffset = target1.x - rawClick1.x * calibrationData.xScale;
	calibrationData.yOffset = target1.y - rawClick1.y * calibrationData.yScale;

	return calibrationData;
}

Vector2<int> Calibrator::getTargetScreenCoordinates(unsigned int targetNumber) {
	if(targetNumber >= 4) return Vector2<int>();

	Vector2<int> target(m_targets[targetNumber].x * m_screenInfo.getRootScreenWidth(), m_targets[targetNumber].y * m_screenInfo.getRootScreenHeight());
	Vector2<int> targetLocal(target.x - m_screenInfo.getX(), target.y - m_screenInfo.getY());
	return targetLocal;
}

std::string Calibrator::GetCoordinateTransformMatrixString(CalibrationData calibrationData) {
	std::stringstream ss;
	ss << calibrationData.xScale << " 0 " << calibrationData.xOffset;
	ss << " 0 " << calibrationData.yScale  << " " << calibrationData.yOffset;
	ss << " 0" << " 0" << " 1";
	return ss.str();
}