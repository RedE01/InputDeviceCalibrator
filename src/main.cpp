#include <GraphicsLibrary.h>
#include "CalibratorApplication.h"

#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, const char** argv) {
	const char* displayName = "";
	const char* outputFileName = "";

	const char* inputDeviceName = "";

	for(int i = 1; i < argc; ++i) {
		const char* arg = argv[i];
		if(std::strcmp(arg, "-d") == 0 || std::strcmp(arg, "--display") == 0) {
			i++;
			if(argc > i) displayName = argv[i];
			else {
				std::cout << "Error: Specify the display name when using the " << arg << " option" << std::endl;
				return 1;
			}
		}
		else if(std::strcmp(arg, "-h") == 0 || std::strcmp(arg, "--help") == 0) {
			std::cout << "Usage: InputDeviceCalibrator [options] input_device_name\n";
			std::cout << "\tOptions are:\n";
			std::cout << "\t-d, --display name\n\t\tSelects the display to use for calibration\n\n";
			std::cout << "\t-h, --help\n\t\tPrints this message\n\n";
			std::cout << "\t-o, --output filename\n\t\tSpecifies the file to print the calibration output to\n\n";
			return 0;
		}
		else if(std::strcmp(arg, "-o") == 0 || std::strcmp(arg, "--output") == 0) {
			i++;
			if(argc > i) outputFileName = argv[i];
			else {
				std::cout << "Error: Specify the output file when using the " << arg << " option" << std::endl;
				return 1;
			}
		}
		else if(strlen(inputDeviceName) == 0) {
			inputDeviceName = argv[i];
		}
		else {
			std::cout << "Error: Unknown option '" << arg << "'" << std::endl;
			return 1;
		}
	}

	if(strlen(inputDeviceName) == 0) {
		std::cout << "Error: No input device specified" << std::endl;
		return 1;
	}

	CalibratorApplication app(inputDeviceName, displayName);
	app.create("InputDeviceCalibrator", rgl::Vector2i(1, 1));
	app.run();

	std::string calibrationString = app.getCalibrationString();

	if(calibrationString.length() > 0) {
		if(std::strlen(outputFileName) > 0) {
			std::ofstream outFile(outputFileName);
			if(!outFile.is_open()) {
				std::cout << "Error: Could not open file '" << outputFileName << "'" << std::endl;
				return 1;
			}

			outFile << "Coordinate Transform Matrix=" << calibrationString;
			outFile.close();

			std::cout << "Coordinate Transform Matrix written to '" << outputFileName << "'" << std::endl;
		}
		else {
			std::cout << "Coordinate Transform Matrix=" << calibrationString << std::endl;
		}
	}
}