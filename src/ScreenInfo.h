#pragma once
#include <memory>
#include <string>

class ScreenInfo {
protected:
	ScreenInfo(const char* screenName);

public:
	static std::unique_ptr<ScreenInfo> Create(const char* screenName);

	const char* getName() const { return m_name.c_str(); }
	bool screenFound() const { return m_screenFound; }
	unsigned int getX() const { return m_x; }
	unsigned int getY() const { return m_y; }
	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }
	unsigned int getRootScreenWidth() const { return m_rootScreenWidth; }
	unsigned int getRootScreenHeight() const { return m_rootScreenHeight; }

protected:
	const std::string m_name;
	bool m_screenFound;
	unsigned int m_x;
	unsigned int m_y;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_rootScreenWidth;
	unsigned int m_rootScreenHeight;
};