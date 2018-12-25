#include "Enc424J600Network.h"

int send_cnt = 0;
int recv_cnt = 0;
unsigned long prev_tick;
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
uint8_t ping[74] =
{
	0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x01,
	0x02, 0x03, 0x04, 0x05, 0x08, 0x00, 0x45, 0x00,
	0x00, 0x3c, 0x2c, 0x0a, 0x00, 0x00, 0x80, 0x01,
	0x00, 0x00, 0xc0, 0xa8, 0x64, 0x01, 0xc0, 0xa8,
	0x64, 0x02, 0x08, 0x00, 0x4d, 0x5a, 0x00, 0x01,
	0x00, 0x01, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x68, 0x69
};

void setup() {
	Serial.begin(9600);
	Enc424J600Network::init((uint8_t*)mac);
	Serial.write("init done\n");
	prev_tick = millis();
}

void loop() {
	unsigned long curr_tick = millis();
	if (curr_tick - prev_tick > 4999) {
		prev_tick = curr_tick;

		// write packet every 5 seconds
		memhandle handle = Enc424J600Network::allocBlock(74);
		Enc424J600Network::writePacket(handle, 0, (uint8_t*)ping, 74);
		Enc424J600Network::sendPacket(handle);
		Enc424J600Network::freeBlock(handle);

		// print number of packets sent
		send_cnt++;
		Serial.println(send_cnt);
	} else {
		// read packet and print via serial
		memhandle handle = Enc424J600Network::receivePacket();
		if (handle != NOBLOCK) {
			uint16_t len = Enc424J600Network::blockSize(handle);
			uint8_t* buf = (uint8_t*)malloc(len);
			Enc424J600Network::readPacket(handle, 0, buf, len);
			recv_cnt ++;
			Serial.println(recv_cnt);
			for (int i = 0; i < len; i ++) {
				char str[5];
				sprintf(str, "0x%02x,", buf[i]);
				Serial.write(str);
				if (i % 16 == 15) {
					Serial.write("\n");
				}
			}
			free(buf);
		}
	}
}
