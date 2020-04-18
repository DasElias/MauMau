#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Main.h"
#include <egui/input/IOHandler.h>

#include <stdexcept>

#include "state/IngameState.h"
#include "state/MainMenuState.h"
#include "state/JoinRoomState.h"
#include "state/CreateRoomState.h"
#include "state/CreateLocalRoomState.h"
#include "state/ParticipantsOverviewState.h"
#include "state/CreditsState.h"

#include "system/GlfwCursorHandler.h"
#include "system/GlfwDisplayHandler.h"
#include "system/GlfwInputHandler.h"


#include <boost/filesystem.hpp>
#include <egui/system/EGuiContext.h>
#include <egui/rendering/NvgRenderer.h>
#include <egui/model/utils/Font.h>
#include <egui/themes/ThemeManager.h>
#include <egui/model/popups/PopupErrorBox.h>
#include <egui\model\utils\Image.h>

#include <shared/utils/Logger.h>

#include "model/LocalGameFacade.h"

#include "renderingModel/Viewport.h"
#include "renderingModel/ProjectionMatrix.h"
#include "rendering/Renderer3D.h"
#include "rendering/CardSceneBackgroundRenderer.h"
#include "rendering/CardStackRenderer.h"
#include "rendering/HandCardStackRenderer.h"
#include "renderingModel/HandCardIntersectionChecker.h"
#include "renderingModel/CardStackIntersectionChecker.h"

#include <shared/utils/ThreadUtils.h>
#include <shared/utils/TimeUtils.h>
#include <shared/utils/MathUtils.h>
#include "utils/FileUtils.h"

#include <iostream>
#include "network/ClientPacketTransmitter.h"
#include "network/ConnectionToServer.h"

#include <shared/packet/stc/GameHasBeenStarted_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasDrawnCards_STCPacket.h>
#include "model/NetworkGameFacade.h"
#include "model/JoinRoomNetworkGameFacade.h"
#include "model/CreateRoomNetworkGameFacade.h"
#include "state/MainMenuNetworkErrorHandler.h"
#include "gui/MauMauTheme.h"

#include <res/font/BoogalooRegular.ttf.h>
#include <res/font/NotoSansBlack.ttf.h>
#include <res/font/NotoSansBold.ttf.h>
#include <res/font/NotoSansExtraBold.ttf.h>
#include <res/font/NotoSansLight.ttf.h>
#include <res/font/NotoSansMedium.ttf.h>
#include <res/font/NotoSansRegular.ttf.h>

using namespace std;
using namespace card;

void glfwErrorCallback(int error, const char* description) {
	log(LogSeverity::FATAL, "--------------- GLFW-ERROR ---------------");
	log(LogSeverity::FATAL, error);
	log(LogSeverity::FATAL, description);
	log(LogSeverity::FATAL, "------------------------------------------");

	switch(error) {
		case GLFW_API_UNAVAILABLE:
		case GLFW_VERSION_UNAVAILABLE:
		case GLFW_PLATFORM_ERROR:
			{
				egui::PopupErrorBox a("MauMau", u8"Ihr PC wird nicht unterstützt. MauMau benötigt mindestens OpenGL 3.3.");
				a.show();
			}
		default:
			{
				egui::PopupErrorBox a("MauMau - Spielabsturz", "Ein fataler, unbekannter Fehler trat auf. \"" + std::string(description) + "\"");
				a.show();
			}
	}

	exit(-1);
}

void cppExceptionCallback() {
	log(LogSeverity::FATAL, "--------------- Exception ----------------");

	if(auto exc = std::current_exception()) {
		// we have an exception
		try {
			std::rethrow_exception(exc);
		} catch(const std::exception& exception) {
			log(LogSeverity::FATAL, exception.what());

			egui::PopupErrorBox a("MauMau - Spielabsturz", "Ein fataler, unbekannter Fehler trat auf. \"" + std::string(exception.what()) + "\"");
			a.show();
		} catch(...) {
			log(LogSeverity::FATAL, "Unknown Exception!");

			egui::PopupErrorBox a("MauMau - Spielabsturz", "Ein fataler, unbekannter Fehler trat auf.");
			a.show();
		}
	}

	log(LogSeverity::FATAL, "------------------------------------------");
	exit(-1);
}


static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if(type == GL_DEBUG_TYPE_OTHER) return;

	log(LogSeverity::FATAL, "------------- OpenGL-Callback ------------");
	log(LogSeverity::FATAL, "message: ");
	log(LogSeverity::FATAL, std::string("\t") + message);
	log(LogSeverity::FATAL, "type: ");
	    switch (type) {
	    case GL_DEBUG_TYPE_ERROR:
			log(LogSeverity::FATAL, "\tERROR");
	        break;
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			log(LogSeverity::FATAL, "\tDEPRECATED_BEHAVIOR");
	        break;
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			log(LogSeverity::FATAL, "\tUNDEFINED_BEHAVIOR");
	        break;
	    case GL_DEBUG_TYPE_PORTABILITY:
			log(LogSeverity::FATAL, "\tPORTABILITY");
	        break;
	    case GL_DEBUG_TYPE_PERFORMANCE:
			log(LogSeverity::FATAL, "\tPERFORMANCE");
	        break;
	}
	log(LogSeverity::FATAL, "id:");
	log(LogSeverity::FATAL, "\t" + std::to_string(id));
	log(LogSeverity::FATAL, "severity:");
	switch (severity) {
	    case GL_DEBUG_SEVERITY_LOW:
			log(LogSeverity::FATAL, "\tLOW");
	        break;
	    case GL_DEBUG_SEVERITY_MEDIUM:
			log(LogSeverity::FATAL, "\tMEDIUM");
	        break;
	    case GL_DEBUG_SEVERITY_HIGH:
			log(LogSeverity::FATAL, "\tHIGH");
	        break;
	    }
	log(LogSeverity::FATAL, "------------------------------------------");

}


namespace card {
	Main::Main() {
		// initialize boost trivial logging
		std::string const logFolder = card::getApplicationFolder() + "\\logs";
		initLogger(logFolder + "\\latest.log", logFolder + "\\fatal.log");

		// log application start
		log(LogSeverity::INFO, "Application has started!");

		// set exception handlers
		std::set_terminate(cppExceptionCallback);
		glfwSetErrorCallback(glfwErrorCallback);

		if(! glfwInit()) throw std::runtime_error("unable to initialize GLFW");

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 16);
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#ifndef NDEBUG
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#else
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		#endif

		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		p_window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "MauMau 3D", NULL, NULL);

		GlfwDisplayHandler displayHandler(p_window, START_WIDTH, START_HEIGHT);
		GlfwInputHandler inputHandler(p_window);
		GlfwCursorHandler cursorHandler(p_window);

		cursorHandler.setCursorPosition(START_WIDTH / 2.0f, START_HEIGHT / 2.0f);
		egui::init(displayHandler, inputHandler, cursorHandler);

		glfwMakeContextCurrent(p_window);
		gladLoadGL();		


		glfwShowWindow(p_window);
		glfwMaximizeWindow(p_window);
		glfwFocusWindow(p_window);

		glfwSwapInterval(VSYNC);

		#ifndef NDEBUG
			glDebugMessageCallback( MessageCallback, 0 );
			glEnable( GL_DEBUG_OUTPUT );
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		#endif
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_DEPTH_CLAMP);
		glClearColor(0, 0, 0, 1);

		const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		const char* model = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		log(LogSeverity::INFO, std::string(vendor) + " | " + model);

		egui::EGuiContext ctx = {};
		egui::NvgRenderer nvgRenderer(ctx);
		egui::Image::setContext(ctx);
		egui::Font::setDefaultFont(egui::Font::createFontFromMemory(ctx, "NotoSans Regular", font_NotoSansRegular, font_NotoSansRegular_size));
		egui::Font::createFontFromMemory(ctx, "NotoSans Medium", font_NotoSansMedium, font_NotoSansMedium_size);
	//	egui::Font::createFontFromMemory(ctx, "NotoSans Semibold", font_notosans);
		egui::Font::createFontFromMemory(ctx, "NotoSans Bold", font_NotoSansBold, font_NotoSansBold_size);
		egui::Font::createFontFromMemory(ctx, "NotoSans ExtraBold", font_NotoSansExtraBold, font_NotoSansExtraBold_size);
		egui::Font::createFontFromMemory(ctx, "NotoSans Black", font_NotoSansBlack, font_NotoSansBlack_size);
		egui::Font::createFontFromMemory(ctx, "NotoSans Light", font_NotoSansLight, font_NotoSansLight_size);
		egui::Font::createFontFromMemory(ctx, "Boogaloo", font_BoogalooRegular, font_BoogalooRegular_size);


		auto theme = std::make_unique<egui::MauMauTheme>(ctx);
		egui::ThemeManager::getInstance().addTheme(theme.get());

		MainMenuNetworkErrorHandler errorHandler(stateManager);
		AvatarTextures avatarTextures;

		IngameState ingameState(stateManager, avatarTextures, nvgRenderer);
		MainMenuState mainMenuState(stateManager, nvgRenderer);
		JoinRoomState joinRoomState(stateManager, avatarTextures, nvgRenderer, errorHandler);
		CreateRoomState createRoomState(stateManager, avatarTextures, nvgRenderer, errorHandler);
		CreateLocalRoomState createLocalRoomState(stateManager, avatarTextures, nvgRenderer);
		ParticipantsOverviewState participantsOverviewState(stateManager, avatarTextures, nvgRenderer);
		CreditsState creditsState(stateManager, nvgRenderer);

		stateManager.addState("IngameState", &ingameState);
		stateManager.addState("MainMenuState", &mainMenuState);
		stateManager.addState("JoinRoomState", &joinRoomState);
		stateManager.addState("CreateRoomState", &createRoomState);
		stateManager.addState("CreateLocalRoomState", &createLocalRoomState);
		stateManager.addState("ParticipantsOverviewState", &participantsOverviewState);
		stateManager.addState("CreditsState", &creditsState);
		stateManager.changeState("MainMenuState");

		loop();

	}

	Main::~Main() {
		glfwDestroyWindow(p_window);
		glfwTerminate();

	}


	void Main::loop() {
		long long lastFrameTime = getMilliseconds();		
		
		while(! glfwWindowShouldClose(p_window)) {
			glViewport(0, 0, egui::getDisplayHandler().getWidth(), egui::getDisplayHandler().getHeight());

			long long thisFrameTime = getMilliseconds();
			float deltaSeconds = (thisFrameTime - lastFrameTime) / 1000.0f;
			lastFrameTime = thisFrameTime;
			this->fps = 1.0f / deltaSeconds;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// update IO handler
			egui::update(deltaSeconds);
			threadUtils_update();

			stateManager.updateAndRender(deltaSeconds);
			glfwSwapInterval(VSYNC);
			glfwSwapBuffers(p_window);
			glfwPollEvents();
		}
	}



}


int main() {
	card::Main();
	return 0;
}
