#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Main.h"
#include <egui/input/IOHandler.h>

#include <stdexcept>

#include "state/IngameState.h"
#include "state/MainMenuState.h"
#include "state/JoinRoomState.h"
#include "state/CreateRoomState.h"
#include "state/ParticipantsOverviewState.h"

#include "system/GlfwCursorHandler.h"
#include "system/GlfwDisplayHandler.h"
#include "system/GlfwInputHandler.h"
#include "system/KeyAssignmentsImpl.h"


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

using namespace std;
using namespace card;

void glfwErrorCallback(int error, const char* description) {
	log(LogSeverity::FATAL, "--------------- GLFW-ERROR ---------------");
	log(LogSeverity::FATAL, error);
	log(LogSeverity::FATAL, description);
	log(LogSeverity::FATAL, "------------------------------------------");
	exit(-1);
}

void cppExceptionCallback() {
	log(LogSeverity::FATAL, "--------------- Exception ----------------");
	egui::PopupErrorBox a("A fatal error has occured!", "A fatal unknown error has happened! Please see the logs for details.");
	a.show();

	if(auto exc = std::current_exception()) {
		// we have an exception
		try {
			std::rethrow_exception(exc);
		} catch(const std::exception& exception) {
			log(LogSeverity::FATAL, exception.what());
		} catch(...) {
			log(LogSeverity::FATAL, "Unknown Exception!");
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
		initLogger(card::getApplicationFolder() + "\\logs");

		// log application start
		log(LogSeverity::INFO, "Application has started!");

		// set exception handlers
		std::set_terminate(cppExceptionCallback);
		glfwSetErrorCallback(glfwErrorCallback);

		if(! glfwInit()) throw std::runtime_error("unable to initialize GLFW");

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		p_window = glfwCreateWindow(START_WIDTH, START_HEIGHT, "VoxelCraft", NULL, NULL);

		GlfwDisplayHandler displayHandler(p_window, START_WIDTH, START_HEIGHT);
		GlfwInputHandler inputHandler(p_window);
		GlfwCursorHandler cursorHandler(p_window);
		KeyAssignmentsImpl keyAssignments;

		cursorHandler.setCursorPosition(START_WIDTH / 2.0f, START_HEIGHT / 2.0f);
		egui::init(displayHandler, inputHandler, cursorHandler, keyAssignments);

		glfwMakeContextCurrent(p_window);
		gladLoadGL();		


		glfwShowWindow(p_window);
		glfwSetWindowPos(p_window, 20, 20);
		glfwFocusWindow(p_window);

		glfwSwapInterval(VSYNC);

		glDebugMessageCallback( MessageCallback, 0 );
		glEnable( GL_DEBUG_OUTPUT );
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		//glEnable(GL_DEPTH_CLAMP);
		glClearColor(0, 0, 0, 1);


		egui::EGuiContext ctx = {};
		egui::NvgRenderer nvgRenderer(ctx);
		egui::Image::setContext(ctx);
		egui::Font::setDefaultFont(egui::Font::createSystemFont(ctx, "Arial"));

		MainMenuNetworkErrorHandler errorHandler(stateManager);

		IngameState ingameState(stateManager, nvgRenderer);
		MainMenuState mainMenuState(stateManager, nvgRenderer);
		JoinRoomState joinRoomState(stateManager, nvgRenderer, errorHandler);
		CreateRoomState createRoomState(stateManager, nvgRenderer, errorHandler);
		ParticipantsOverviewState participantsOverviewState(stateManager, nvgRenderer);

		stateManager.addState("IngameState", &ingameState);
		stateManager.addState("MainMenuState", &mainMenuState);
		stateManager.addState("JoinRoomState", &joinRoomState);
		stateManager.addState("CreateRoomState", &createRoomState);
		stateManager.addState("ParticipantsOverviewState", &participantsOverviewState);
		stateManager.changeState("MainMenuState");
/*

		//TODO remove
#define LOCAL
#ifdef CREATE
		auto gameFacade = std::make_shared<CreateRoomNetworkGameFacade>(errorHandler, "c");
#endif // CREATE
#ifdef JOIN
		auto gameFacade = std::make_shared<JoinRoomNetworkGameFacade>(errorHandler, "j", 0);
#endif
#ifdef LOCAL
		auto gameFacade = std::make_shared<LocalGameFacade>("a");
		gameFacade->getRoom().requestGameStart();
#endif

		stateManager.setGameFacade(gameFacade);
	*/	
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

			auto gameFacade = stateManager.getGameFacade();
	/*		auto gameFacade = std::dynamic_pointer_cast<NetworkGameFacade>(stateManager.getGameFacade());
			if(gameFacade->isRoomAvailable()) {
				if(gameFacade->getRoom().canStartGame()) {
					gameFacade->getRoom().requestGameStart();
				}
			}*/

			/*if(egui::getInputHandler().isKeyDown(KEY_1)) {
				viewport.setPosition(viewport.getPosition() + glm::vec3(0, 0, 0.05f));
			}
			if(egui::getInputHandler().isKeyDown(KEY_2)) {
				viewport.setPosition(viewport.getPosition() - glm::vec3(0, 0, 0.05f));
			}
			if(egui::getInputHandler().isKeyDown(KEY_3)) {
				viewport.setPosition(glm::vec3(0, 0, 5));
			}

			if(egui::getInputHandler().isKeyDown(KEY_4)) {
				viewport.setRotation(viewport.getRotation() + glm::vec3(0.01f, 0, 0));
			}
			if(egui::getInputHandler().isKeyDown(KEY_5)) {
				viewport.setRotation(viewport.getRotation() - glm::vec3(0.01f, 0, 0));
			}
			if(egui::getInputHandler().isKeyDown(KEY_6)) {
				viewport.setRotation(glm::vec3(-PI / 8, 0, 0));
			}
			if(egui::getInputHandler().isKeyDown(KEY_7)) {
				//toRender.addFromPlain(Card(1));
			}
			static bool f = false;
			if(egui::getInputHandler().isKeyDown(KEY_8) && !f)  {
				game.proxyPlayerGetCardsLocal("123", 1);
				game.proxyPlayerGetCardsLocal("456", 1);
				game.proxyPlayerGetCardsLocal("789", 1);
				game.localPlayerGetCardsLocal({{Card::HEART_EIGHT}});
				f = true;
			}
			if(egui::getInputHandler().isKeyDown(KEY_9)) {
				f = false;

			}
			if(egui::getInputHandler().isKeyDown(KEY_0) && !f) {
				game.playCardLocal("123", Card::CLUB_ACE, CardIndex::NULLINDEX);
				f = true;
			}
			 */

			long long thisFrameTime = getMilliseconds();
			float deltaSeconds = (thisFrameTime - lastFrameTime) / 1000.0f;
			lastFrameTime = thisFrameTime;
			this->fps = 1.0f / deltaSeconds;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			// update IO handler
			egui::update(deltaSeconds);
			threadUtils_update();

			stateManager.updateAndRender(deltaSeconds);

			glfwSwapInterval((CardAnimator::arePendingAnimations()) ? VSYNC : VSYNC_NO_ANIMATION_PENDING);
			glfwSwapBuffers(p_window);
			glfwPollEvents();
		}
	}



}


int main() {
	card::Main();
	return 1;
}
