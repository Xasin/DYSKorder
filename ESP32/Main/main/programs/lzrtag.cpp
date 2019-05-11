/*
 * lzrtag.cpp
 *
 *  Created on: 3 May 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"
#include "../core/segments.h"

#include "ValueBox.h"

using namespace DSKY;
using namespace DSKY::Prog;
using namespace DSKY::Seg;

namespace Programs {
	program_exit_t lzrFunction(const CommandChunk &cmd) {
		if(DSKY::mqtt.is_disconnected() == 255) {
			DSKY::console.printf("Connect wifi first!\n");
			return DSKY::Prog::FAIL;
		}

		std::string target = DSKY::Prog::Program::get_input("Target:");

		if(target == "")
			return DSKY::Prog::OK;

		if(target == "status") {
			auto statusSub = Xasin::MQTT::Subscription(DSKY::mqtt, "Lasertag/Players/#");
			statusSub.on_received = [](Xasin::MQTT::MQTT_Packet data) {
				std::string plName = data.topic.substr(0, data.topic.find("/"));
				std::string paramTopic = data.topic.substr(data.topic.find("/")+1);

				printf("Got: %s %s\n", plName.data(), paramTopic.data());

				if(paramTopic != "System")
					return;

				DSKY::console.printf("Update for: %s\n", plName.data());
			};

			DSKY::Prog::Program::wait_for_esc();
			return DSKY::Prog::OK;
		}

		auto pingBox = Peripheral::OLED::ValueBox(60, 11, &DSKY::display);
		pingBox.offsetX = 64; pingBox.offsetY = 30;
		pingBox.setup(0, 18, "", "/18");

		auto actualPingBox = Peripheral::OLED::ValueBox(60, 11, &DSKY::display);
		actualPingBox.offsetX = 64; actualPingBox.offsetY = 42;
		actualPingBox.setup(0, 150, "ms");

		seg_a.param_type = DisplayParam::INT;
		auto ammoSub = Xasin::MQTT::Subscription(DSKY::mqtt, "Lasertag/Players/" + target + "/Ammo");
		ammoSub.on_received = [&pingBox](Xasin::MQTT::MQTT_Packet data) {
			seg_a.value = reinterpret_cast<const uint32_t*>(data.data.data())[0];
			pingBox.set_value(seg_a.value);
		};
		auto pingSub = Xasin::MQTT::Subscription(DSKY::mqtt, "Lasertag/Players/" + target + "/System");
		pingSub.on_received = [&actualPingBox](Xasin::MQTT::MQTT_Packet data) {
			actualPingBox.set_value(reinterpret_cast<const uint32_t*>(data.data.data())[2]);
		};

		while(true) {
			std::string cmdStr = DSKY::Prog::Program::get_input("LZR>");
			if(cmdStr == "")
				return DSKY::Prog::OK;

			auto lzrCMD = DSKY::Prog::CommandChunk(cmdStr);
			auto param  = lzrCMD.get_cmd();

			if(param == "b")
				param = "FX/Brightness";
			if(param == "t")
				param = "Team";
			if(param == "m")
				param = "FX/Marked";

			if(lzrCMD.get_arg_count() == 1) {
				auto argStr = lzrCMD.get_arg_str(0);
				DSKY::mqtt.publish_to("Lasertag/Players/" + target + "/" + param, argStr.data(), argStr.length());
			}
		}

		return DSKY::Prog::OK;
	}

	void lzr_init() {
		new Program("lzr", lzrFunction);
	}
}
