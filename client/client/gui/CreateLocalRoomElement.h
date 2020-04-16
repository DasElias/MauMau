#pragma once
#include "BasicRoomCreationElement.h"
#include "LabeledInputField.h"
#include "LabeledIntegerSelector.h"
#include "AvatarChooser.h"
#include "EditOptionsElement.h"

namespace card {
	class CreateLocalRoomElement : public BasicRoomCreationElement {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<AvatarChooser> avatarChooser;
			std::shared_ptr<EditOptionsElement> optionsElement;
			std::shared_ptr<LabeledInputField> usernameInputField;
			std::shared_ptr<LabeledIntegerSelector> amountOfOpponentsInputField;

			RoomOptions options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateLocalRoomElement(AvatarTextures& avatarTextures);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameInput() const;
			int getAmountOfOpponents() const;
			Avatar getSelectedAvatar() const;
			RoomOptions getOptions() const;
			
			void setUsernameInput(std::string username);
			void setSelectedAvatar(Avatar avatar);
	};
}