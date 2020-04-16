#include "CategorizedAvatarGrid.h"
#include <shared/model/AvatarUtils.h>

namespace card {
	CategorizedAvatarGrid::CategorizedAvatarGrid(AvatarTextures& avatarTextures, Avatar chosenAvatar) :
			VBox(),
			selectedAvatar(chosenAvatar) {
		int const PADDING_PX = 30;		
		int const PADDING_BETWEEN_TITLE_AND_CONTENT = 5;

		uncategorizedLabel = std::make_shared<egui::Label>("Avatar nicht gesetzt");
		addChildElement(uncategorizedLabel);
		formatLabel(uncategorizedLabel);

		std::vector<Avatar> uncategorizedGridContent = {{0}};
		uncategorizedGrid = std::make_shared<AvatarGrid>(avatarTextures, uncategorizedGridContent, chosenAvatar);
		addChildElement(uncategorizedGrid);
		uncategorizedGrid->setPreferredHeight({1 * uncategorizedGrid->getHeightPerAvatar(), egui::RelativityMode::RELATIVE_IN_PARENT});
		uncategorizedGrid->setYTranslation(0 * PADDING_PX + 1 * PADDING_BETWEEN_TITLE_AND_CONTENT);
		uncategorizedGrid->setOnSelectHandler([this](Avatar a) {
			maleGrid->clearSelectedAvatar();
			femaleGrid->clearSelectedAvatar();
			selectedAvatar = a;
		});

		maleLabel = std::make_shared<egui::Label>(u8"Männliche Avatare");
		addChildElement(maleLabel);
		formatLabel(maleLabel);
		maleLabel->setYTranslation(1 * PADDING_PX + 1 * PADDING_BETWEEN_TITLE_AND_CONTENT);

		maleGrid = std::make_shared<AvatarGrid>(avatarTextures, getAllMaleAvatars(), chosenAvatar);
		addChildElement(maleGrid);
		maleGrid->setPreferredHeight({1 * maleGrid->getHeightPerAvatar(), egui::RelativityMode::RELATIVE_IN_PARENT});
		maleGrid->setYTranslation(1 * PADDING_PX + 2 * PADDING_BETWEEN_TITLE_AND_CONTENT);
		maleGrid->setOnSelectHandler([this](Avatar a) {
			uncategorizedGrid->clearSelectedAvatar();
			femaleGrid->clearSelectedAvatar();
			selectedAvatar = a;
		});

		femaleLabel = std::make_shared<egui::Label>("Weibliche Avatare");
		addChildElement(femaleLabel);
		formatLabel(femaleLabel);
		femaleLabel->setYTranslation(2 * PADDING_PX + 2 * PADDING_BETWEEN_TITLE_AND_CONTENT);

		femaleGrid = std::make_shared<AvatarGrid>(avatarTextures, getAllFemaleAvatars(), chosenAvatar);
		addChildElement(femaleGrid);
		femaleGrid->setPreferredHeight({1 * femaleGrid->getHeightPerAvatar(), egui::RelativityMode::RELATIVE_IN_PARENT});
		femaleGrid->setYTranslation(2 * PADDING_PX + 3 * PADDING_BETWEEN_TITLE_AND_CONTENT);
		femaleGrid->setOnSelectHandler([this](Avatar a) {
			maleGrid->clearSelectedAvatar();
			uncategorizedGrid->clearSelectedAvatar();
			selectedAvatar = a;
		});

	}
	Avatar CategorizedAvatarGrid::getSelectedAvatar() const {
		return selectedAvatar;
	}
	void CategorizedAvatarGrid::setSelectedAvatar(Avatar a) {
		selectedAvatar = a;
		maleGrid->trySetSelectedAvatar(a);
		femaleGrid->trySetSelectedAvatar(a);
		uncategorizedGrid->trySetSelectedAvatar(a);
		
	}
	void CategorizedAvatarGrid::formatLabel(std::shared_ptr<egui::Label> lbl) {
		int const FONT_SIZE_PX = 25;
		egui::Color const color = {0.62, 0.62, 0.62};

		lbl->getTextComponent()->setColor(color);
		lbl->getTextComponent()->setHorizontalAlignment(egui::Text::HorizontalAlignment::CENTER);
		lbl->getTextComponent()->setFontSize(FONT_SIZE_PX, false);
		lbl->getTextComponent()->setFont(egui::Font::getFont("NotoSans Medium"));
		lbl->setPreferredHeight({FONT_SIZE_PX, egui::RelativityMode::ABSOLUTE_VALUE});
		lbl->setPreferredWidth({1, egui::RelativityMode::RELATIVE_IN_PARENT});

		auto border = std::make_shared<egui::Border>(color);
		border->setBottomType(egui::BorderType::SOLID);
		lbl->setBorder(border);
	}
}