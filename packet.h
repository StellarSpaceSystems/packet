#ifndef PACKET_H
#define PACKET_H

#ifdef ARDUINO
#include "Arduino.h"
#elseif
#include <cstring>
#endif

enum PacketType {
  HEARTBEAT = 0x01,
  TELEMETRY = 0x02,
  IGNITION = 0x03,
  SHUTDOWN = 0x04,
  ACK = 0x05,
  CONF = 0x06,
  INFO = 0x07
};

struct {
  const char* type;
  char data;
} DecodeException;

class Packet {
private:
  PacketType type;
  char data[245];

public:
  Packet();

  int length() {return 10 + (int) strlen(this->data);}

  PacketType get_type() {return this->type;}
  void set_type(PacketType type) {this->type = type;}

  char* get_data() {return this->data;}
  void set_data(char data[]) {strcpy(this->data,data);}

  void encode(char* buf);

  static void fletcher16(char* buf, char* in);
  static Packet* decode(char* data);
  static void hexs(char *buf, unsigned int in);
};

#endif
