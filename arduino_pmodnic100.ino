#include "Enc424J600Network.hpp"

unsigned int send_cnt = 0;
unsigned int recv_cnt = 0;
unsigned long prev_tick;
unsigned long curr_tick;
uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
unsigned int test_pkt_len = 74;
uint8_t test_pkt[74] =
{
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
	0x02, 0x03, 0x04, 0x05, 0x08, 0x00, 0x45, 0x00,
	0x00, 0x3c, 0x2c, 0x0a, 0x00, 0x00, 0x80, 0x01,
	0x00, 0x00, 0xc0, 0xa8, 0x64, 0x02, 0xc0, 0xa8,
	0x64, 0x01, 0x08, 0x00, 0x4d, 0x5a, 0x00, 0x01,
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
  char str[128];
	curr_tick = millis();
	if (curr_tick - prev_tick > 4999) {
		prev_tick = curr_tick;

		// write packet every 5 seconds
		memhandle handle = Enc424J600Network::allocBlock(test_pkt_len);
		if (handle != NOBLOCK) {
			Enc424J600Network::writePacket(handle, 0, test_pkt, test_pkt_len);
			Enc424J600Network::sendPacket(handle);
			Enc424J600Network::freeBlock(handle);

			// print number of packets sent
			send_cnt++;
			sprintf(str, "#%u ping sent\n", send_cnt);
			Serial.write(str);
		} else {
			Serial.write("cannot allocated transmit buffer\n");
		}
	} else {
		// read packet and print via serial
		memhandle handle = Enc424J600Network::receivePacket();
		if (handle != NOBLOCK) {
			uint16_t len = Enc424J600Network::blockSize(handle);
			uint8_t* buf = (uint8_t*)malloc(len);
			Enc424J600Network::readPacket(handle, 0, buf, len);
			recv_cnt ++;
			sprintf(str, "pkt #%u (len=%u) : \n", recv_cnt, len);
			Serial.write(str);
			for (int i = 0; i < len; i ++) {
				sprintf(str, "%02x,", buf[i]);
				Serial.write(str);
				if (i % 32 == 31) {
					Serial.write("\n");
				}
			}
      Serial.write("\n");
			free(buf);
		}
	}
}
