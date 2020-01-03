#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace card {
	class Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static std::string const PACKET_TYPE_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int packetType;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			Packet(int packetType);
			Packet(const Packet&) = default;
			virtual ~Packet() = default;

		// ----------------------------------------------------------------------
		// ---------------------------STATIC-METHODS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int getPacketType(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getJson() const;
			int getPacketType() const;

		protected:
			void throwErrorIfTypeIsIncorrect(nlohmann::json& jsonHandle) const;

			virtual void addJsonProperties(nlohmann::json& jsonHandle) const;
	};
}