
require 'opus-ruby'

require 'mqtt/sub_handler'

$mqtt = MQTT::SubHandler.new('localhost');

SAMPLERATE = 24000; # In Samples/Second
BITRATE    = 16000; # In kBit/Second

FRAME_SIZE = 0.06;  # In Milliseconds

FILENAME = "test.wav";

PACKET_SIZE_BYTES = (FRAME_SIZE * BITRATE / 8.0).ceil;

opus_encoder = Opus::Encoder.new(SAMPLERATE, SAMPLERATE * FRAME_SIZE, 1);
opus_encoder.vbr_rate = 0;
opus_encoder.bitrate = BITRATE;

file_read = `sox #{FILENAME} -r #{SAMPLERATE} -c 1 -b 16 -e signed -t raw -`;
file_read.force_encoding('ASCII-8BIT');

read_num_samples = file_read.size() / 2;
read_length  = read_num_samples / SAMPLERATE.to_f;

num_opus_packets = (read_length / FRAME_SIZE).floor;

puts "Read #{read_length}s of Audio, turning it into #{num_opus_packets} packets!";

output_packets = "";

num_opus_packets.times do
	frame_buffer = file_read.slice!(0, FRAME_SIZE * SAMPLERATE * 2);
	opus_data = opus_encoder.encode(frame_buffer, frame_buffer.size);
	output_packets += opus_data;

	$mqtt.publish_to "DSKorder/Audio", opus_data, qos: 0;

	sleep FRAME_SIZE;
end

puts "Produced #{output_packets.length} bytes, packet length is #{PACKET_SIZE_BYTES} bytes";
puts "Outputting as C-Array";

output_text = <<~EOT

#include <stdint.h>

#ifndef OPUS_AUDIO_BUNDLE_DEF
#define OPUS_AUDIO_BUNDLE_DEF
struct opus_audio_bundle_t {
	uint16_t samplerate;
	uint16_t packetsize;
	uint32_t num_packets;
	const char * data;
};
#endif

const opus_audio_bundle_t audio_pack = {
	#{SAMPLERATE}, #{PACKET_SIZE_BYTES}, #{output_packets.length / PACKET_SIZE_BYTES}, (const uint8_t[]){
EOT

newline_counter = 10000;

output_packets.each_byte do |b|
	if((newline_counter += 1) > 8)
		output_text << "\n     ";
		newline_counter = 0;
	end

	output_text << b.to_s << ", ";
end

output_text << "\n}};";

File.open(FILENAME + ".h", "w") do |file|
	file.write(output_text);
end
