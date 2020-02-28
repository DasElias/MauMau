#include "MauMauTheme.h"
#include "MauMauTheme.h"

#include "../utils/FileUtils.h"
#include <egui/model\nodes\Labeled.h>
#include <egui/model\nodes\Button.h>
#include <egui/model\nodes\InputField.h>
#include <egui/model/nodes/KeySelectButton.h>
#include <egui/model\nodes\Slider.h>
#include "ColoredButton.h"
#include "IntegerSelector.h"

#include <egui/model/utils/LinearGradient.h>
#include <egui/model/nodeComponents/background/ColoredBackground.h>
#include <egui/model/nodeComponents/background/TexturedBackground.h>

#include <egui/model/nodeComponents/background/LinearGradientBackground.h>
#include <stdexcept>

float const egui::MauMauTheme::BTN_HEIGHT = 0.065f;

egui::MauMauTheme::MauMauTheme(EGuiContext& context) :
	AbstractTheme(context, [this]() {
		std::string applFolder = card::getApplicationFolder();

		// LABELED
	/*	std::shared_ptr<Font> defaultFont = Font::createFont(ctx, "CodersCrux", std::string(applFolder).append("\\resources\\fonts\\coders_crux.ttf"));
		std::shared_ptr<Font> emojiFont = Font::createFont(ctx, "EmojiFallback", std::string(applFolder).append("\\resources\\fonts\\OpenSansEmoji.ttf"));
		defaultFont->addFallbackFont(ctx, emojiFont);
		AbstractTheme::addComponent(Labeled::getClassName_static(), [&defaultFont, &emojiFont](Node* p_node) {
			Labeled* p_labeled = dynamic_cast<Labeled*>(p_node);
			if(!p_labeled) throw std::logic_error("Component is not of type Labeled!");

			p_labeled->getTextComponent()->setFont(Font::getFont("CodersCrux"));
		});*/

		// Colored Button
		AbstractTheme::addComponent(card::ColoredButton::getClassName_static(), button_coloredButton_apply());

		// Button
		AbstractTheme::addComponent(Button::getClassName_static(), button_keySelectButton_apply());

		// KeySelectButton
		AbstractTheme::addComponent(KeySelectButton::getClassName_static(), button_keySelectButton_apply());

		// InputField
		AbstractTheme::addComponent(InputField::getClassName_static(), inputField_apply());

		// integerSelector
		AbstractTheme::addComponent(card::IntegerSelector::getClassName_static(), integerSelector_apply());

	}) {
}

std::function<void(egui::Node* const)> egui::MauMauTheme::button_coloredButton_apply() {
	return [this](Node* const p_node) {
		using namespace card;
		ColoredButton* p_btn = dynamic_cast<ColoredButton*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type ColoredButton");

		p_btn->setDisabledBackground(std::make_shared<LinearGradientBackground>(
			LinearGradient(
				Color(0.57f, 0.57f, 0.57f),
				Color(0.45f, 0.45f, 0.45f),
				0.5f, 0,
				0.5f, 1
			), 1.0f
		), RENDER_EXCLUSIVELY);

		switch(p_btn->getButtonType()) {
			case ColoredButtonType::GREEN:
				p_btn->setBackground(std::make_shared<ColoredBackground>(
					Color(0.46f, 0.87f, 0.54f)
				));
				p_btn->setHoveredBackground(std::make_shared<LinearGradientBackground>(
					LinearGradient(
						Color(0.46f, 0.87f, 0.54f),
						Color(0.62f, 0.91f, 0.68f),
						0.5f,
						0,
						0.5f,
						1
					)
					), RENDER_EXCLUSIVELY);
				break;
			case ColoredButtonType::BLUE:
				p_btn->setBackground(std::make_shared<ColoredBackground>(
					Color(0.0f, 0.69f, 0.94)
				));
				p_btn->setHoveredBackground(std::make_shared<LinearGradientBackground>(
					LinearGradient(
						Color(0.0f, 0.69f, 0.94f),
						Color(0.34f, 0.83f, 1.0f),
						0.5f,
						0,
						0.5f,
						1
					)
					), RENDER_EXCLUSIVELY);
				break;
			case ColoredButtonType::RED:
				p_btn->setBackground(std::make_shared<ColoredBackground>(
					Color(0.93f, 0.38f, 0.29f)
				));
				p_btn->setHoveredBackground(std::make_shared<LinearGradientBackground>(
					LinearGradient(
						Color(0.93f, 0.38f, 0.29f),
						Color(0.95f, 0.54f, 0.47f),
						0.5f,
						0,
						0.5f,
						1
					)
				), RENDER_EXCLUSIVELY);
				break;
		}
	};
}

std::function<void(egui::Node* const)> egui::MauMauTheme::button_keySelectButton_apply() {
	return [this](Node* const p_node) {
		Labeled* p_btn = dynamic_cast<Labeled*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type Button or KeySelectButton!");

		// set dimensions
		p_btn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {BTN_HEIGHT, RelativityMode::RELATIVE_ON_SCREEN});
		p_btn->setRadius(0.2f, true);

		p_btn->getTextComponent()->setForceOneLine(true);
		p_btn->getTextComponent()->setFontSize(0.5f, true);
		p_btn->getTextComponent()->setColor(Color(1, 1, 1));

		float const colorPoint1 = 20.0f / 256;
		float const colorPoint2 = 60.0f / 256;

		std::shared_ptr<Background> bg(new LinearGradientBackground(LinearGradient(
			Color(colorPoint1, colorPoint1, colorPoint1),
			Color(colorPoint2, colorPoint2, colorPoint2),
			0.5f, 1,
			0.5f, 0
		)));
		p_btn->setBackground(bg);

		std::shared_ptr<Border> border(new Border(Color(), SOLID));
		p_btn->setBorder(border);

		// disabled
		std::shared_ptr<Background> disabledBg(new LinearGradientBackground(LinearGradient(
			Color(colorPoint1, colorPoint1, colorPoint1, 0.5f),
			Color(colorPoint2, colorPoint2, colorPoint2, 0.5f),
			0.5f, 1,
			0.5f, 0
		)));
		p_btn->setDisabledBackground(disabledBg, RENDER_EXCLUSIVELY);

		std::shared_ptr<Border> disabledBorder(new Border(Color(0, 0, 0, 0.5f), SOLID));
		p_btn->setDisabledBorder(disabledBorder, RENDER_EXCLUSIVELY);

		p_btn->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		p_btn->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);
	};
}

std::function<void(egui::Node* const)> egui::MauMauTheme::inputField_apply() {
	return[this](Node * const p_node) {
		InputField* p_btn = dynamic_cast<InputField*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type InputField!");

		// set dimensions
		p_btn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {BTN_HEIGHT, RelativityMode::RELATIVE_ON_SCREEN});

		p_btn->getTextComponent()->setForceOneLine(true);
		p_btn->getTextComponent()->setFontSize(0.5f, true);
		p_btn->getTextComponent()->setColor(Color(1, 1, 1));

		std::shared_ptr<Background> bg(new ColoredBackground(Color(0, 0, 0)));
		p_btn->setBackground(bg);

		std::shared_ptr<Border> border(new Border(Color(0.627f, 0.627f, 0.627f), SOLID));
		border->setWidth(2);
		p_btn->setBorder(border);

		p_btn->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		p_btn->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);
	};
}

std::function<void(egui::Node* const)> egui::MauMauTheme::integerSelector_apply() {
	return[this](Node* const p_node) {
		card::IntegerSelector* p_btn = dynamic_cast<card::IntegerSelector*>(p_node);
		if(!p_btn) throw std::logic_error("Component is not of type IntegerSelector!");
/*
		// set dimensions
		p_btn->setPreferredDimension({1, RelativityMode::RELATIVE_IN_PARENT}, {BTN_HEIGHT, RelativityMode::RELATIVE_ON_SCREEN});

		p_btn->getTextComponent()->setForceOneLine(true);
		p_btn->getTextComponent()->setFontSize(0.5f, true);
		p_btn->getTextComponent()->setColor(Color(1, 1, 1));

		std::shared_ptr<Background> bg(new ColoredBackground(Color(0, 0, 0)));
		p_btn->setBackground(bg);

		std::shared_ptr<Border> border(new Border(Color(0.627f, 0.627f, 0.627f), SOLID));
		border->setWidth(2);
		p_btn->setBorder(border);

		p_btn->getTextComponent()->setVerticalAlignment(Text::VerticalAlignment::MIDDLE);
		p_btn->getTextComponent()->setHorizontalAlignment(Text::HorizontalAlignment::CENTER);*/
	};
}



