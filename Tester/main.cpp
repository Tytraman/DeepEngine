#include <stdio.h>
#include <string>

#include <DE/def.h>
#include <DE/core.hpp>
#include <DE/error.hpp>
#include <DE/window.hpp>
#include <DE/pixel.hpp>
#include <DE/triangle.hpp>
#include <DE/key.hpp>
#include <DE/sys_win.hpp>
#include <DE/logger.hpp>
#include <DE/debug.hpp>
#include <DE/vec.hpp>
#include <DE/mat.hpp>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define TARGET_FPS    16

#define AUTHORS		"Tytraman"
#define BUILD_VER	"dev build 0.0.1"

de::Logger *g_Logger = nullptr;

void eventCallback(de::Window &window, de::devent e) {
	// Appelle le callback par défaut
	de::Window::defaultInputCallback(window, e);
}

void updateCallback(de::Window &window) {
	de::Drawable *pixel = window.getDrawable(0);
	de::Drawable *t1 = window.getDrawable(1);

	de::Debug::addFunctionToCallbackList(DE_FUNCTION_NAME);

	if(de::Key::isPressed(de::key::LeftArrow)) {
		pixel->move(de::direction::Left, 5);
		t1->rotate(-5.0f);
	}

	if(de::Key::isPressed(de::key::RightArrow)) {
		pixel->move(de::direction::Right, 5);
		t1->rotate(5.0f);
	}

	if(de::Key::isPressed(de::key::UpArrow)) {
		pixel->move(de::direction::Up, 5);
	}

	if(de::Key::isPressed(de::key::DownArrow)) {
		pixel->move(de::direction::Down, 5);
	}

	de::Debug::removeFunctionFromCallbackList();
}

#undef main
int main() {
	de::ErrorStatus errorStatus;

	DWORD o = de::SysWin::getTlsIndex();

	errorStatus = de::Core::init();
	if(errorStatus != de::ErrorStatus::NoError) {
		de::DError::printError(errorStatus);
		return 1;
	}

	de::Logger logger("supralog.log");
	if(!logger.open()) {
		fprintf(stderr, "Error when creating logger\n");
		de::Core::quit();
		return 1;
	}
	g_Logger = &logger;

	de::Debug::addFunctionToCallbackList(DE_FUNCTION_NAME);
	de::Debug::writeToStream(logger);

	de::Window window(TARGET_FPS);
	window.setEventCallback(eventCallback);
	window.setUpdateCallback(updateCallback);
	errorStatus = de::Window::create(window, "DeepEngine Tester by " AUTHORS " [" BUILD_VER "]", de::size(WINDOW_WIDTH, WINDOW_HEIGHT));
	if(errorStatus != de::ErrorStatus::NoError) {
		de::DError::printError(errorStatus);
		de::Core::quit();
		return 1;
	}

	de::Pixel pixel(window.getRendererPtr(), de::vec2(10, 10), de::colora(255, 0, 0, 255));
	de::Triangle t1(window.getRendererPtr(), de::vec2(300, 300), de::colora(255, 0, 255, 255), 100.0f);

	printf(
		"AB: %.2f\n"
		"AC: %.2f\n"
		"BC: %.2f\n"
		"Angle A: %.2f\n"
		"Angle B: %.2f\n"
		"Angle C: %.2f\n"
		"Total angles: %.2f\n",
		
		t1.getLengthOfAB(), t1.getLengthOfAC(), t1.getLengthOfBC(),
		t1.getAngleA(), t1.getAngleB(), t1.getAngleC(),
		t1.getAngleA() + t1.getAngleB() + t1.getAngleC()
	);

	window.addDrawable(&pixel);
	window.addDrawable(&t1);

	// Lance la boucle du jeu, bloquant
	window.run();

	window.destroy();

	logger.close();
	de::Core::quit();

	printf("Good-bye!\n");
	return 0;
}
