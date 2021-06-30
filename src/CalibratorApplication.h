#pragma once
#include <GraphicsLibrary.h>
#include <string>

class Calibrator;

class CalibratorApplication : public rgl::Application {
public:
	CalibratorApplication(const char* inputDeviceName, const char* monitorName);
	~CalibratorApplication();

	virtual void onInit() override;
	virtual void onUpdate() override;
	virtual void onEvent(rgl::Event* event) override;

	std::string getCalibrationString() const { return m_calibrationString; }

private:
	void redraw();
	void drawTextBox();
	void drawTarget(int x, int y, int width, int height);

private:
	const std::string m_inputDeviceName;
	const std::string m_monitorName;
	std::unique_ptr<Calibrator> m_calibrator;
	std::string m_calibrationString;
};