#include "ParticipantsOverviewElement.h"
#include "ParticipantsOverviewElement.h"

#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>

#include <egui/model/positioning/RelativePositioningInParent.h>
#include <res/menu/participantsOverview.png.h>

namespace card {
	ParticipantsOverviewElement::ParticipantsOverviewElement(AvatarTextures& avatarTextures) :
			avatarTextures(avatarTextures) {
		int const TITLE_FONT_SIZE_PX = 30;
		int const ROOM_CODE_FONT_SIZE_PX = 90;
		

		float const MARGIN_LEFT_RIGHT = 0.015f;
		float const MARGIN_TOP = 0.01f;
		auto pageContentBox = std::make_shared<egui::VBox>();
		addChildElement(pageContentBox);
		pageContentBox->setPreferredWidth({1 - 2*MARGIN_LEFT_RIGHT, egui::RelativityMode::RELATIVE_IN_PARENT});
		pageContentBox->setOwnPositioning(std::make_shared<egui::RelativePositioningInParent>(MARGIN_LEFT_RIGHT, MARGIN_TOP));
		auto titleBox = std::make_shared<egui::VBox>();
		pageContentBox->addChildElement(titleBox);

		buttonBox = std::make_shared<egui::HBox>();

		participantsBox = std::make_shared<egui::HBox>();
		pageContentBox->addChildElement(participantsBox);
		pageContentBox->addChildElement(buttonBox);

		float const BUTTON_BOX_HEIGHT = 0.25f;
		float const SPACE_BETWEEN_TITLE_AND_CONTENT = 0.1f;
		titleBox->setPreferredHeight({{TITLE_FONT_SIZE_PX + ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}, {SPACE_BETWEEN_TITLE_AND_CONTENT, egui::RelativityMode::RELATIVE_IN_PARENT}});
		participantsBox->setPreferredHeight({{1 - MARGIN_TOP - SPACE_BETWEEN_TITLE_AND_CONTENT - BUTTON_BOX_HEIGHT, egui::RelativityMode::RELATIVE_IN_PARENT}, {-TITLE_FONT_SIZE_PX - ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE}});
		buttonBox->setPreferredHeight({BUTTON_BOX_HEIGHT, egui::RelativityMode::RELATIVE_IN_PARENT});

		auto title = std::make_shared<egui::Label>("Raum-Code", TITLE_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		title->setPreferredHeight({TITLE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		titleBox->addChildElement(title);
		this->roomCodeLabel = std::make_shared<egui::Label>("212", ROOM_CODE_FONT_SIZE_PX, false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::MIDDLE, egui::Color(1, 1, 1));
		roomCodeLabel->setPreferredHeight({ROOM_CODE_FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		roomCodeLabel->setYTranslation(-10);
		titleBox->addChildElement(roomCodeLabel);


		this->leaveRoomBtn = addButton(
			egui::Image::loadFromMemory(tex_participantsOverviewState_quitGame, tex_participantsOverviewState_quitGame_size), 
			egui::Image::loadFromMemory(tex_participantsOverviewState_quitGameHover, tex_participantsOverviewState_quitGameHover_size)
		);
		this->editOptionsBtn = addButton(
			egui::Image::loadFromMemory(tex_participantsOverviewState_editOptions, tex_participantsOverviewState_editOptions_size),
			egui::Image::loadFromMemory(tex_participantsOverviewState_editOptionsHover, tex_participantsOverviewState_editOptionsHover_size)
		);
		this->viewOptionsBtn = addButton(
			egui::Image::loadFromMemory(tex_participantsOverviewState_viewOptions, tex_participantsOverviewState_viewOptions_size),
			egui::Image::loadFromMemory(tex_participantsOverviewState_viewOptionsHover, tex_participantsOverviewState_viewOptionsHover_size)
		);
		this->addAiPlayerBtn = addButton(
			egui::Image::loadFromMemory(tex_participantsOverviewState_addAiPlayer, tex_participantsOverviewState_addAiPlayer_size),
			egui::Image::loadFromMemory(tex_participantsOverviewState_addAiPlayerHover, tex_participantsOverviewState_addAiPlayerHover_size)
		);
		this->startGameBtn = addButton(
			egui::Image::loadFromMemory(tex_participantsOverviewState_startGame, tex_participantsOverviewState_startGame_size),
			egui::Image::loadFromMemory(tex_participantsOverviewState_startGameHover, tex_participantsOverviewState_startGameHover_size)
		);

		participantsBox->setSpaceBetweenElements({5, egui::RelativityMode::ABSOLUTE_VALUE});
		participantsBox->setShouldCenterElements(true);
		buttonBox->setSpaceBetweenElements({0.04f, egui::RelativityMode::RELATIVE_IN_PARENT});
		buttonBox->setShouldCenterElements(true);

		editOptionsElement = std::make_shared<EditOptionsElement>();
		addChildElement(editOptionsElement);
		editOptionsElement->setVisible(false);
		editOptionsElement->setConsumeAllMouseEvents(true);
		editOptionsElement->addContinueBtnEventHandler({[this](egui::ActionEvent&) {
			editOptionsElement->setVisible(false);
		}});
		editOptionsElement->addBackBtnEventHandler({[this](egui::ActionEvent&) {
			editOptionsElement->setVisible(false);
		}});
		editOptionsBtn->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			editOptionsElement->setVisible(true);
		}});

		viewOptionsElement = std::make_shared<ViewOptionsElement>();
		addChildElement(viewOptionsElement);
		viewOptionsElement->setVisible(false);
		viewOptionsElement->setConsumeAllMouseEvents(true);
		viewOptionsElement->addBackBtnEventHandler({[this](egui::ActionEvent&) {
			viewOptionsElement->setVisible(false);
		}});
		viewOptionsBtn->getMouseClickedEventManager().addEventHandler({[this](egui::MouseEvent&) {
			viewOptionsElement->setVisible(true);
		}});
	}
	std::shared_ptr<egui::AspectRatioElement> ParticipantsOverviewElement::addButton(std::shared_ptr<egui::Image> texImage, std::shared_ptr<egui::Image> hoverImage) {
		float const BTN_HEIGHT_IN_PARENT = 0.75f;

		auto button = std::make_shared<egui::AspectRatioElement>(1.0f);
		buttonBox->addChildElement(button);
		button->setMaxHeight({BTN_HEIGHT_IN_PARENT, egui::RelativityMode::RELATIVE_IN_PARENT});
		button->setBackground(std::make_shared<egui::TexturedBackground>(texImage));
		button->setHoveredBackground(std::make_shared<egui::TexturedBackground>(hoverImage), egui::RENDER_EXCLUSIVELY);
		button->setDisabledBackground(std::make_shared<egui::ColoredBackground>(egui::Color(1, 1, 1, 0.15f)), egui::RENDER_ADDITIONALLY);
		button->setRadius(0.1f, true);
		button->setBorder(std::make_shared<egui::Border>(egui::Color(0.71f, 0.71f, 0.71f), egui::BorderType::SOLID, 1.0f));
		button->setShouldConsiderIfNotVisible(false);
		return button;
	}
	void ParticipantsOverviewElement::update(ProxyRoom& room) {
		roomCodeLabel->setText(std::to_string(room.getRoomCode()));

		static std::vector<std::shared_ptr<ParticipantOnClient>> participantsInLastFrame = {};
		static std::shared_ptr<ParticipantOnClient> roomLeaderInLastFrame = {};
		static std::map<std::string, int> roomOptionsInLastFrame = {};
		auto participants = room.getAllParticipants();
		if(participants != participantsInLastFrame || room.getRoomLeader() != roomLeaderInLastFrame) {
			participantsInLastFrame = participants;
			roomLeaderInLastFrame = room.getRoomLeader();
			rebuildParticipants(room);
		}

		auto options = room.getOptions();
		if(options.getAllOptions() != roomOptionsInLastFrame) {
			roomOptionsInLastFrame = options.getAllOptions();
			editOptionsElement->loadOptions(options);
			viewOptionsElement->loadOptions(options);
		}

		addAiPlayerBtn->setDisabled(! room.canAiPlayerJoin());
		startGameBtn->setDisabled(! room.canStartGame());
		editOptionsBtn->setVisible(room.isLocalUserLeader());
		viewOptionsBtn->setVisible(! room.isLocalUserLeader());

	}
	void ParticipantsOverviewElement::addOptionsChangeEventHandler(std::function<void(RoomOptions)> handler) {
		editOptionsElement->addContinueBtnEventHandler({[this, handler](egui::ActionEvent&) {
			RoomOptions options = editOptionsElement->getOptions();
			handler(options);
		}});
	}
	void ParticipantsOverviewElement::rebuildParticipants(ProxyRoom& room) {
		participantsBox->clearChildElements();

		for(auto& p : room.getAllParticipants()) {
			ParticipantEntry::EntryAttributes attributes = 0;

			if(room.isLeader(p)) attributes |= ParticipantEntry::ROOM_LEADER;
			if(p->isAiPlayer()) attributes |= ParticipantEntry::AI_PLAYER;
			if(room.isLocalUser(p)) attributes |= ParticipantEntry::LOCAL_PLAYER;
			if(room.isLocalUserLeader() && !room.isLeader(p)) attributes |= ParticipantEntry::CAN_BE_KICKED;
			if(room.canBeRoomLeader(p)) attributes |= ParticipantEntry::CAN_BE_PROMOTED;

			auto elem = std::make_shared<ParticipantEntry>(avatarTextures, p->getUsername(),p->getAvatar(), attributes);
			participantsBox->addChildElement(elem);
			elem->addKickHandler([this, p]() {
				kickHandler(p);
			});
			elem->addPromoteHandler([this, p]() {
				promoteHandler(p);
			});
		}
	}
	void ParticipantsOverviewElement::addAiPlayerJoinHandler(std::function<void(void)> handler) {
		addAiPlayerBtn->getMouseClickedEventManager().addEventHandler({[handler](egui::MouseEvent& e) {
			handler();
		}});
	}
	void ParticipantsOverviewElement::addGameStartHandler(std::function<void(void)> handler) {
		startGameBtn->getMouseClickedEventManager().addEventHandler({[handler](egui::MouseEvent& e) {
			handler();
		}});
	}
	void ParticipantsOverviewElement::addLeaveRoomHandler(std::function<void(void)> handler) {
		leaveRoomBtn->getMouseClickedEventManager().addEventHandler({[handler](egui::MouseEvent& e) {
			handler();
		}});
	}
	void ParticipantsOverviewElement::setKickEventHandler(std::function<void(std::shared_ptr<ParticipantOnClient>)> handler) {
		this->kickHandler = handler;
	}
	void ParticipantsOverviewElement::setPromoteEventHandler(std::function<void(std::shared_ptr<ParticipantOnClient>)> handler) {
		this->promoteHandler = handler;
	}
}