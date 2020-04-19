#include "BottomMessageRenderer.h"
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/LinearGradientBackground.h>
#include <egui/model/nodes/UnorganizedParentElement.h>
#include <egui/model/nodes/VBox.h>
#include <egui/model/positioning/RelativePositioningInParent.h>
#include <egui/model/positioning/RelativePositioningOnScreen.h>
#include <shared/utils/MathUtils.h>
#include <shared/utils/TimeUtils.h>

namespace card {
	float const BottomMessageRenderer::MESSAGE_WIDTH_ON_SCREEN = 0.2f;
	float const BottomMessageRenderer::MESSAGE_RIGHT_MARGIN_ON_SCREEN = 0.02f;
	int const BottomMessageRenderer::PADDING_BETWEEN_MESSAGE_BOXES_PX = 5;
	int const BottomMessageRenderer::MESSAGE_ANIMATION_DURATION_MS = 100;

	BottomMessageRenderer::TextToRender::TextToRender(int fontSizePx, egui::MasterRenderer& eguiRenderer) :
			eguiRenderer(eguiRenderer),
			fontSizePx(fontSizePx) {
		this->text = std::make_shared<egui::Label>("", float(fontSizePx), false, egui::Text::HorizontalAlignment::LEFT, egui::Text::VerticalAlignment::TOP, egui::Color(1, 1, 1));
		addChildElement(text);


		setPreferredHeight({float(fontSizePx), egui::RelativityMode::ABSOLUTE_VALUE});
		text->setXTranslation(X_TRANSLATION);
		text->setYTranslation(Y_TRANSLATION);
		text->setStretchX(-2 * X_TRANSLATION);
		text->setStretchY(-2 * Y_TRANSLATION);

		coloredBackground = std::make_shared<egui::ColoredBackground>(egui::Color(0.22f, 0.22f, 0.22f));
		setBackground(coloredBackground);

		auto border = std::make_shared<egui::Border>(egui::Color(0.94f, 0.58f, 0.14f), egui::BorderType::DISABLED, 5);
		border->setLeftType(egui::BorderType::SOLID);
		setBorder(border);

		auto positioning = std::make_shared<egui::RelativePositioningInParent>(0.0f, 0.0f);
		text->setOwnPositioning(positioning);
	}
	void BottomMessageRenderer::TextToRender::setText(std::string text) {
		this->text->setText(text);
		int lastCharY = eguiRenderer.getCharPositionInTextBox(this->text->getTextComponent(), text.size() - 1).second - egui::y_percentToPixel(this->text->getAbsYMargin());
		setPreferredHeight({float(lastCharY + fontSizePx + 2 * Y_TRANSLATION), egui::RelativityMode::ABSOLUTE_VALUE});
	}
	void BottomMessageRenderer::TextToRender::updateOpacity(long long messageAppendUnixTimeMs) {
		const float x1 = 0;
		const float y1 = 0;
		const float x2 = MESSAGE_ANIMATION_DURATION_MS;
		const float y2 = 1;
		const float x = getMilliseconds() - messageAppendUnixTimeMs;
		float opacity = interpolateLinearAndClamp(x, x1, y1, x2, y2);

		this->text->getTextComponent()->setColor({1, 1, 1, opacity});

		egui::Color bgColor = coloredBackground->getBackgroundColor();
		bgColor.setAlpha(opacity);
		this->coloredBackground->setBackgroundColor(bgColor);

		egui::Color borderColor = getBorder()->getLeftColor();
		borderColor.setAlpha(opacity);
		getBorder()->setLeftColor(borderColor);
	}
	void BottomMessageRenderer::TextToRender::updateXTranslation(long long messageAppendUnixTimeMs) {
		const float x1 = 0;
		const float y1 = -MESSAGE_WIDTH_ON_SCREEN;
		const float x2 = MESSAGE_ANIMATION_DURATION_MS;
		const float y2 = 0;
		const float x = getMilliseconds() - messageAppendUnixTimeMs;

		float xTranslation_onScreen = interpolateLinearAndClamp(x, x1, y1, x2, y2);

		int xTranslation_px = egui::x_percentToPixel(xTranslation_onScreen);
		this->setXTranslation(xTranslation_px);
	}
	BottomMessageRenderer::BottomMessageRenderer(egui::MasterRenderer& eguiRenderer) :
			eguiRenderer(eguiRenderer),
			parentPositioning(std::make_shared<egui::RelativePositioningOnScreen>(MESSAGE_RIGHT_MARGIN_ON_SCREEN, 0)) {

		int const FONT_SIZE_PX = 25;
		auto parent = std::make_shared<egui::VBox>();
		for(int i = 0; i < textBoxesArray.size(); i++) {
			auto textBox = std::make_shared<TextToRender>(FONT_SIZE_PX, eguiRenderer);
			parent->addChildElement(textBox);

			textBox->setVisible(false);
			textBoxesArray[i] = textBox;
		}

		scene.setRootElement(parent);
		parent->setPreferredWidth({MESSAGE_WIDTH_ON_SCREEN, egui::RelativityMode::RELATIVE_ON_SCREEN});
		parent->setOwnPositioning(parentPositioning);
		parent->setSpaceBetweenElements({PADDING_BETWEEN_MESSAGE_BOXES_PX, egui::RelativityMode::ABSOLUTE_VALUE});
	}
	void BottomMessageRenderer::render(const MessageQueue& msgQueue) {
		makeAllTextBoxesInvisible();

		float height = 0;
		std::vector<Message> messages = msgQueue.getLastVisibleMessages();
		for(int i = messages.size() - 1; i >= 0; i--) {
			auto textBox = textBoxesArray[i];
			Message& msg = messages[i];
			std::string msgContent = msg.content;

			textBox->setText(msgContent);
			textBox->updateOpacity(msg.appendUnixTimeMs);
			textBox->updateXTranslation(msg.appendUnixTimeMs);
			textBox->setVisible(true);

			height += textBox->getComputedHeight() + egui::y_pixelToPercent(PADDING_BETWEEN_MESSAGE_BOXES_PX);
		}

		parentPositioning->setY(0.825f - height);

		eguiRenderer.beginFrame();
		scene.render(eguiRenderer);
		eguiRenderer.endFrame();
	}

	void BottomMessageRenderer::makeAllTextBoxesInvisible() {
		for(auto& textBox : textBoxesArray) {
			textBox->setVisible(false);
		}
	}
	
}