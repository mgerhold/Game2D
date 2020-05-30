#include <iostream>
#include "Application.h"
#include "ScopedProfiler.h"



int main() {
	try {
		Application app;
		app.run();
	} catch (const std::exception& e) {
		std::cout << "CAUGHT EXCEPTION: " << e.what() << "\n";
		std::cin.get();
	}
	ScopedProfiler::printMeasurements();
}