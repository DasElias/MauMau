#include "KeyAssignmentsImpl.h"
#include <egui/input/InputHandler.h>
#include <sstream>

#include "../utils/FileUtils.h"
#include <boost/filesystem.hpp>

namespace card {
	KeyAssignmentsImpl::KeyAssignmentsImpl() {

		SI_Error err = properties.LoadFile(getFile().c_str());
		if(err < 0) throw std::runtime_error("Couldn't load key assignments file.");

		bool save = false;

		save |= setProperty_impl("MOVE_FORWARD", KEY_W, true);
		save |= setProperty_impl("MOVE_LEFT", KEY_A, true);
		save |= setProperty_impl("MOVE_RIGHT", KEY_D, true);
		save |= setProperty_impl("MOVE_BACKWARD", KEY_S, true);

		save |= setProperty_impl("OPEN_INVENTORY", KEY_E, true);

		save |= setProperty_impl("SLOT1", KEY_1, true);
		save |= setProperty_impl("SLOT2", KEY_2, true);
		save |= setProperty_impl("SLOT3", KEY_3, true);
		save |= setProperty_impl("SLOT4", KEY_4, true);
		save |= setProperty_impl("SLOT5", KEY_5, true);
		save |= setProperty_impl("SLOT6", KEY_6, true);
		save |= setProperty_impl("SLOT7", KEY_7, true);
		save |= setProperty_impl("SLOT8", KEY_8, true);
		save |= setProperty_impl("SLOT9", KEY_9, true);

		save |= setProperty_impl("JUMP", KEY_SPACE, true);
		save |= setProperty_impl("BREAK_BLOCK", MOUSE_BUTTON_2, true);
		save |= setProperty_impl("PLACE_BLOCK", MOUSE_BUTTON_1, true);

		if(save) {
			properties.SaveFile(getFile().c_str());
		}
	}

	std::string KeyAssignmentsImpl::getFile() {
		std::string file = getApplicationFolder().append("\\config.ini");
		
		if(! boost::filesystem::exists(file)) createFile(file);

		return file;
	}

	void KeyAssignmentsImpl::save() {
		properties.SaveFile(getFile().c_str());
	}

	int KeyAssignmentsImpl::getProperty(std::string key) {
		long fromProperties = properties.GetLongValue("voxelcraft", key.c_str(), -1);

		if(fromProperties == -1) {
			throw std::invalid_argument("the key doesn't exist");
		} else return int(fromProperties);
	}

	void KeyAssignmentsImpl::setProperty(std::string key, int value) {
		setProperty_impl(key, value);
		save();
	}

	bool KeyAssignmentsImpl::setProperty_impl(std::string key, int value, bool preventReplace) {
		bool shouldInsert = true;
		if(preventReplace && properties.GetLongValue("voxelcraft", key.c_str(), -1) != -1) {
			shouldInsert = false;
		}

		if(shouldInsert) return properties.SetLongValue("voxelcraft", key.c_str(), value) == SI_INSERTED;
		else return false;
	}
}
