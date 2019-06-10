/*
 * utilities.cpp
 *
 *  Created on: 3 May 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"

#include "../core/segments.h"

using namespace DSKY;
using namespace DSKY::Prog;

namespace Programs {

program_exit_t consoleRelay(const CommandChunk &cmd) {
	if(DSKY::mqtt.is_disconnected() == 255) {
	    auto wifiEnableChunk = DSKY::Prog::CommandChunk("wifi home");
	    DSKY::Prog::Program::find(wifiEnableChunk)->run(wifiEnableChunk);
	}

    DSKY::mqtt.subscribe_to("DSKorder/Console/In", [](const Xasin::MQTT::MQTT_Packet data) {
    	DSKY::BTN::last_btn_event = {};
    	DSKY::BTN::last_btn_event.enter = true;

    	DSKY::BTN::last_btn_event.typed = data.data;
    	DSKY::BTN::last_btn_event.typed_char = '\n';

    	xTaskNotify(Program::programTask, 1, eSetBits);
    }, 2);

    DSKY::mqtt.subscribe_to("DSKorder/Console/BTN", [](const Xasin::MQTT::MQTT_Packet data) {
    	DSKY::BTN::last_btn_event = {};

    	DSKY::BTN::last_btn_event.typed_char = data.data[0];

    	if(data.data[0] == '\n')
    		DSKY::BTN::last_btn_event.enter = true;
    	else if(data.data[0] == 27)
    		DSKY::BTN::last_btn_event.escape = true;

    	xTaskNotify(Program::programTask, 1, eSetBits);
    }, 2);

	return Prog::OK;
}

program_exit_t wifiFunc(const CommandChunk &cmd) {
	if(cmd.get_arg_str(0) == "lzr") {
		Xasin::MQTT::Handler::start_wifi("Lasertag\0", "\0");
		DSKY::mqtt.start("mqtt://192.168.250.1");
		DSKY::console.printf("LZR WiFi starting!\n");
	}
	else if(cmd.get_arg_str(0) == "home") {
		Xasin::MQTT::Handler::start_wifi("TP-LINK_84CDC2\0", "f36eebda48\0");
		mqtt.start("mqtt://uMFNQh4CursTSbkTb1m8MBqg08CJYuBi56TJcN9w5qdLrntreFpR4DVUznrnGTtz@mqtt.flespi.io");
		console.printf("Home WiFi starting!\n");
	}
	else if(cmd.get_arg_str(0) == "uni") {
		Xasin::MQTT::Handler::start_wifi_enterprise("eduroam\0", "\0", "FQP-EQP\0", "eduroam@uni-hannover.de\0", "Wrong\0");
		mqtt.start("mqtt://test.mosquitto.org");
	}

	return Prog::OK;
}

program_exit_t volumeFunc(const CommandChunk &cmd) {
	if(cmd.get_arg_count() == 0) {
		console.printf("Volume is: %d\n", audio.volumeMod);
		return Prog::OK;
	}

	audio.volumeMod = cmd.get_arg_flt(0, 100);
	console.printf("Volume set to: %d\n", audio.volumeMod);
	return Prog::OK;
}

program_exit_t i2c_scanner(const CommandChunk &cmd) {
	uint8_t i2c_no = cmd.get_arg_flt(0, 0);
	if(i2c_no > 1)
		i2c_no = 1;

	console.printf("Scanning I2C no. %d\n", i2c_no);

	DSKY::Seg::seg_b.param_type = DSKY::Seg::DisplayParam::HEX;
	for(uint8_t i=0; i<127; i++) {
		DSKY::Seg::seg_b.value = i;
		if(XaI2C::MasterAction::poke(i) == ESP_OK)
			console.printf("%d, ", i);

		vTaskDelay(10);
	}

	return Prog::OK;
}

void util_init() {
	new Program("wifi", wifiFunc, false);
	new Program("vol", volumeFunc, false);
	new Program("i2c", i2c_scanner, true);

	new Program("consoleRelay", consoleRelay, false);
}
}
