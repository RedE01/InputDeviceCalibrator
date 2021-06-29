#include <GraphicsLibrary.h>

#include <iostream>

class CalibratorApplication : public rgl::Application {
public:
	CalibratorApplication() {}

	virtual void onInit() override {
	}

	virtual void onUpdate() override {
	}

	virtual void onEvent(rgl::Event* event) override {
		if(event->GetEventType() == rgl::EventType::KeyPressed && ((rgl::KeyPressedEvent*)event)->key == rgl::Key::Esc) {
			window->windowOpen = false;
		}
		if(event->GetEventType() == rgl::EventType::WindowExpose) redraw();

		if(event->GetEventType() == rgl::EventType::MouseButtonPressed && ((rgl::MouseButtonPressedEvent*)event)->button == rgl::MouseButton::LEFT) {
			std::cout << ((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.x << ", " << ((rgl::MouseButtonPressedEvent*)event)->rootWindowCoords.y << std::endl;
		}
	}

private:
	void redraw() {
		window->clearScreen();
	}
};

int main(int argc, const char** argv) {
	CalibratorApplication app;
	app.create("InputDeviceCalibrator", rgl::Vector2i(720, 480));
	app.run();
}