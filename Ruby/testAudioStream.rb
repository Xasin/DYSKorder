# Useful PACTL command:
# pactl load-module module-pipe-sink sink_name=DSKorder rate=48000 channels=1 channel_map=mono file=/tmp/dskorder_audio.out


require 'mqtt/sub_handler'
require 'socket'

require 'opus-ruby'

SAMPLERATE = 48000; # In Samples/Second
BITRATE    = 24000; # In kBit/Second

FRAME_SIZE = 0.02;  # In Milliseconds

$mqtt = MQTT::SubHandler.new('mqtt://192.168.178.230');

opus_encoder = Opus::Encoder.new(SAMPLERATE, SAMPLERATE * FRAME_SIZE, 1);
opus_encoder.vbr_rate = 0;
opus_encoder.bitrate = BITRATE;

#rec  = IO.popen("arecord -r #{SAMPLERATE} -f S16_LE -R 0 --period-size 1024",
#	:external_encoding=>"ASCII-8BIT");
#rec.binmode

rec = File.open("/tmp/dskorder_audio.out", "rb:ASCII-8BIT:ASCII-8BIT");

dString = "";
dString.force_encoding('ASCII-8BIT');

next_read_time = Time.now();

loop do
	out_data = [3].pack "C";
	3.times do
		sleep_tdiff = next_read_time - Time.now();
		sleep sleep_tdiff if sleep_tdiff > 0
		next_read_time = Time.now() + FRAME_SIZE;

		rec.read(SAMPLERATE * FRAME_SIZE * 2, dString);
		out_data += opus_encoder.encode(dString, dString.size);
	end

	$mqtt.publish_to("/esp32/dragon-cookie/E0.E2.E6.56.14.D0/audio/play", out_data, qos: 0);
end

def send_packet(packet_number)
	packet_signal = [packet_number];

	2048.times do |i|
		packet_signal << 50*Math::sin(2 * Math::PI * 600.0 * (i + 2048*packet_number)/16000.0);
	end

	$mqtt.publish_to("DSKorder/Audio", packet_signal.pack("L<c*"), qos: 0);
end

$mqtt.subscribe_to "DSKorder/Audio/Rewind" do |data|
	data = data.unpack("c")[0];

	return if(data > $current_packet)

	(data...$current_packet).each do |i|
		send_packet i
	end
end
