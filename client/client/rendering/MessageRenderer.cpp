#include "MessageRenderer.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/LinearGradientBackground.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/positioning/RelativePositioningInParent.h>

namespace card {
	MessageRenderer::TextToRender::TextToRender(int fontSizePx) {
		this->text = std::make_shared<egui::Label>("", float(fontSizePx), false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::TOP, egui::Color(1, 1, 1), egui::Font::getFont("Boogaloo"));
		this->shadow = std::make_shared<egui::Label>("", float(fontSizePx), false, egui::Text::HorizontalAlignment::CENTER, egui::Text::VerticalAlignment::TOP, egui::Color(0, 0, 0), egui::Font::getFont("Boogaloo"));
		shadow->setXTranslation(7);
		shadow->setYTranslation(7);
		addChildElement(shadow);
		addChildElement(text);
		setPreferredHeight({float(fontSizePx + 7), egui::RelativityMode::ABSOLUTE_VALUE});

		float const TOP_OPACITY = 0.9f;
		float const BOTTOM_OPACITY = 0.3f;
		coloredBackground = std::make_shared<egui::ColoredBackground>(egui::Color(0, 0, 0, TOP_OPACITY));
		linearGradientBackground = std::make_shared<egui::LinearGradientBackground>(
			egui::LinearGradient(
				{0, 0, 0, TOP_OPACITY},
				{0, 0, 0, BOTTOM_OPACITY},
				0.5f,
				0,
				0.5f,
				1
			)
		);

		auto positioning = std::make_shared<egui::RelativePositioningInParent>(0.0f, 0.0f);
		text->setOwnPositioning(positioning);
		shadow->setOwnPositioning(positioning);
	}
	void MessageRenderer::TextToRender::setText(std::string text) {
		this->text->setText(text);
		this->shadow->setText(text);
	}
	void MessageRenderer::TextToRender::updateBackground(bool isLastElement) {
		if(isLastElement) text->setBackground(linearGradientBackground);
		else text->setBackground(coloredBackground);
	}
	MessageRenderer::MessageRenderer(egui::MasterRenderer& eguiRenderer) :
			eguiRenderer(eguiRenderer) {

		int const FONT_SIZE_PX = 55;
		auto parent = std::make_shared<egui::VBox>();
		for(int i = 0; i < textBoxesArray.size(); i++) {
			auto textBox = std::make_shared<TextToRender>(FONT_SIZE_PX);
			parent->addChildElement(textBox);

			textBox->setVisible(false);
			textBoxesArray[i] = textBox;
		}

		scene.setRootElement(parent);
	}
	void MessageRenderer::render(const MessageQueue& msgQueue) {
		makeAllTextBoxesInvisible();

		std::vector<Message> messages = msgQueue.getLastVisibleMessages();
		for(int i = 0; i < messages.size(); i++) {
			auto textBox = textBoxesArray[i];
			Message& msg = messages[i];
			std::string msgContent = msg.content;

			textBox->setText(msgContent);
			bool isLastMessageElement = (i == (messages.size() - 1));
			textBox->updateBackground(isLastMessageElement);
			textBox->setVisible(true);
		}

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}

	void MessageRenderer::makeAllTextBoxesInvisible() {
		for(auto& textBox : textBoxesArray) {
			textBox->setVisible(false);
		}
	}
	
}