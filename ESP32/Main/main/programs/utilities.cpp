/*
 * utilities.cpp
 *
 *  Created on: 3 May 2019
 *      Author: xasin
 */

#include "../program/Program.h"
#include "../core/core.h"

#include "../core/segments.h"

#include "AudioStream.h"

#include <lwip/sockets.h>

#include "esp_log.h"
#include "opus.h"

Xasin::Peripheral::AudioStream testStream = Xasin::Peripheral::AudioStream(DSKY::audio, DSKY::mqtt);
OpusDecoder * test_decoder = nullptr;

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

void audio_rec_task(void *args) {
	auto buffer = new std::array<int8_t, 512>();

	sockaddr_in dest_addr;
	dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(3689);

	int rec_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	bind(rec_sock, reinterpret_cast<sockaddr *>(&dest_addr), sizeof(dest_addr));

	while(true) {
		int len = recvfrom(rec_sock, buffer->data(), buffer->size()-1, 0, nullptr, nullptr);

		// testStream.add_samples(buffer->data(), len);
	}
}

program_exit_t wifiFunc(const CommandChunk &cmd) {
	if(cmd.get_arg_str(0) == "lzr") {
		Xasin::MQTT::Handler::start_wifi("Lasertag\0", "\0");
		DSKY::mqtt.start("mqtt://192.168.250.1");
		DSKY::console.printf("LZR WiFi starting!\n");
	}
	else if(cmd.get_arg_str(0) == "flespi") {
		Xasin::MQTT::Handler::start_wifi("TP-LINK_84CDC2\0", "f36eebda48\0", -1);
		mqtt.start("mqtt://uMFNQh4CursTSbkTb1m8MBqg08CJYuBi56TJcN9w5qdLrntreFpR4DVUznrnGTtz@mqtt.flespi.io");
		console.printf("Flespi WiFi starting!\n");
	}
	else if(cmd.get_arg_str(0) == "home" || cmd.get_arg_str(0).size() == 0) {
		Xasin::MQTT::Handler::start_wifi("TP-LINK_84CDC2\0", "f36eebda48\0", -1);

		mqtt.start("mqtt://192.168.6.64");
		console.printf("Home WiFi starting!\n");
	}
	else if(cmd.get_arg_str(0) == "uni") {
		Xasin::MQTT::Handler::start_wifi_enterprise("eduroam\0", "\0", "FQP-EQP\0", "eduroam@uni-hannover.de\0", "Wrong\0");
		mqtt.start("mqtt://test.mosquitto.org");
	}

	mqtt.subscribe_to("DSKorder/Ping", [](const Xasin::MQTT::MQTT_Packet packet) {
		mqtt.publish_to("DSKorder/PingAnswer", packet.data.data(), packet.data.size(), false, 0);
	});

	DSKY::audio.insert_sample(&testStream);

	test_decoder = opus_decoder_create(24000, 1, nullptr);

	mqtt.subscribe_to("DSKorder/Audio", [](const Xasin::MQTT::MQTT_Packet packet) {
		int16_t audio_data[5760] = {};

		auto sample_ptr = reinterpret_cast<const uint8_t*>(packet.data.data());
		uint8_t no_samples = *sample_ptr;
		sample_ptr++;

		uint16_t packet_length = (packet.data.length() - 1) / no_samples;

		while(no_samples != 0) {
			size_t processed = opus_decode(test_decoder, sample_ptr, packet_length, audio_data, 5760, 0);

			sample_ptr += no_samples;

			no_samples--;

			testStream.add_samples(audio_data, processed);
		}


		// ESP_LOGI("OPUS", "Decoded %d input bytes into %d samples", packet.data.size(), processed);
	});

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


struct speech_param_t {
	uint32_t speech_duration;
	uint32_t beginning_ticks;
	uint32_t end_ticks;
};

const speech_param_t speech_profiles[] = {
		{
				75*6*1000,
				60*1000,
				60*1000,
		},
		{
				35*6*1000,
				30*1000,
				0,
		}
};
#define CROSSED(time) ((current_tick >= (time)) && (last_tick <= (time)))
program_exit_t speech_timer(const CommandChunk &cmd) {
	int speech_type = cmd.get_arg_flt(0, 0);

	if(speech_type > 1)
		speech_type = 1;

	auto speech = speech_profiles[speech_type];

	TickType_t start_time = xTaskGetTickCount();
	uint32_t last_tick = 0;
	DSKY::Seg::seg_a.param_type = DSKY::Seg::DisplayParam::INT;
	DSKY::Seg::seg_a.fixComma = 2;


	while(xTaskGetTickCount() - start_time < speech.speech_duration) {
		uint32_t current_tick = xTaskGetTickCount() - start_time;

		if(current_tick >= speech.beginning_ticks && last_tick <= speech.beginning_ticks) {
			Xasin::Trek::play(Xasin::Trek::INPUT_REQ);
		}

		if(CROSSED(speech.speech_duration - speech.end_ticks)) {
			Xasin::Trek::play(Xasin::Trek::INPUT_REQ);
		}

		if(CROSSED(speech.speech_duration - 30*1000))
			Xasin::Trek::play(Xasin::Trek::INPUT_BAD);
		if(CROSSED(speech.speech_duration - 10*1000))
			Xasin::Trek::play(Xasin::Trek::PROG_FAILED);


		uint32_t speech_sec = ((speech.speech_duration - current_tick) / 1000)%60;
		uint32_t speech_mins = ((speech.speech_duration - current_tick)/60000);

		DSKY::Seg::seg_a.value = speech_mins + 0.01*speech_sec;

		DSKY::Prog::Program::wait_for_button(100);
		if(DSKY::BTN::last_btn_event.enter) {
			return DSKY::Prog::OK;
		}

		last_tick = current_tick;
	}

	return DSKY::Prog::FAIL;
}

void util_init() {
	new Program("wifi", wifiFunc, false);
	new Program("vol", volumeFunc, false);
	new Program("i2c", i2c_scanner, true);

	new Program("consoleRelay", consoleRelay, false);

	new Program("speech", speech_timer, true);

	wifiFunc(CommandChunk("wifi"));

	printf("=== FREE HEAP ATM IS: %d", esp_get_minimum_free_heap_size());
}
}
