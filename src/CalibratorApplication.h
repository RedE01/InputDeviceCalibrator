#pragma once
#include <GraphicsLibrary.h>

class Calibrator;

class CalibratorApplication : public rgl::Application {
public:
	CalibratorApplication(const char* monitorName);
	~CalibratorApplication();

	virtual void onInit() override;
	virtual void onUpdate() override;
	virtual void onEvent(rgl::Event* event) override;

private:
	void redraw();
	void drawTextBox();
	void drawTarget(int x, int y, int width, int height);

private:
	const char* m_monitorName;
};