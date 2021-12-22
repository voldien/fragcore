
#include <Core/RefPtr.h>
#include <Window/Display.h>
#include <Window/WindowManager.h>
#include <gtest/gtest.h>


using namespace fragcore;

// TODO relocate to fragview client seperate code section.

TEST(WindowTest, Create_WindowManager_No_Throw) { ASSERT_NO_THROW(WindowManager::getInstance()); }

TEST(DisplayTest, Create_Window_No_Exception) {}

TEST(DisplayTest, Window_Set_Title_Correct) {}

TEST(DisplayTest, Display_Check_Monitor) {
	// Display dislay;
}

TEST(DisplayTest, Display) {
	// 	SDL_Init(SDL_INIT_VIDEO);
	// 	int nDevices = SDLDisplay::getNumDisplays();
	// 	ASSERT_GE(nDevices, 0);

	// 	for (int i = 0; i < nDevices; i++) {
	// 		SDLDisplay display = SDLDisplay(i);
	// 		Display::DPI dpi;
	// 		ASSERT_GE(display.width(), 0);
	// 		ASSERT_GE(display.height(), 0);
	// 		ASSERT_GE(display.refreshRate(), 0);
	// 		ASSERT_NO_THROW(display.getDPI(&dpi));
	// 		ASSERT_GE(dpi.ddpi, 0.0f);
	// 		ASSERT_GE(dpi.hdpi, 0.0f);
	// 		ASSERT_GE(dpi.vdpi, 0.0f);

	// 		std::vector<Display::Mode> modes;
	// 		ASSERT_NO_THROW(modes = display.getModes());
	// 		for (int j = 0; j > modes.size(); j++) {
	// 			ASSERT_GE(modes[i].width, 0);
	// 			ASSERT_GE(modes[i].height, 0);
	// 			ASSERT_GE(modes[i].refreshRate, 0);
	// 			ASSERT_GE(modes[i].format, 0);
	// 		}
	// 	}

	// //	fragview::Display display = fragview::Display::getDisplay(0);
	// //	display.getModes();

	// 	SDL_Quit();
}

TEST(DisplayTest, Window_) {
	// SDL_Init(SDL_INIT_VIDEO);

	// SDLRendererWindow *rendererWindow;

	// SDL_Quit();
}

TEST(DisplayTest, Window) {}
