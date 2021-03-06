#include "packet.h"


#if defined(ARDUINO)
#include "Arduino.h"
#else
#include <cstring>
#include <cstdio>
#include <stdint.h>
#include <cstdlib>
#endif

Packet::Packet() {

}

void Packet::encode(char* out) {
  char chunk1[this->length() + 1] = {};
  strcat(chunk1, "#");
  char buf[100] = {};
  this->hexs(buf, this->length());
  strcat(chunk1, buf);
  this->hexs(buf, this->type);
  strcat(chunk1, buf);
  strcat(chunk1, this->data);
  strcpy(out, chunk1);
  this->fletcher16(buf, chunk1);
  strcat(out, buf);
  strcat(out, ";");
}

void Packet::fletcher16(char* buf, char* in)
{
  uint8_t const *data = reinterpret_cast<const uint8_t*>(&in[0]);
  size_t bytes = strlen(in);
  uint16_t sum1 = 0xff, sum2 = 0xff;

  while (bytes) {
          size_t tlen = bytes > 20 ? 20 : bytes;
          bytes -= tlen;
          do {
                  sum2 += sum1 += *data++;
          } while (--tlen);
          sum1 = (sum1 & 0xff) + (sum1 >> 8);
          sum2 = (sum2 & 0xff) + (sum2 >> 8);
  }
  /* Second reduction step to reduce sums to 8 bits */
  sum1 = (sum1 & 0xff) + (sum1 >> 8);
  sum2 = (sum2 & 0xff) + (sum2 >> 8);
  uint16_t out = sum2 << 8 | sum1;
  char a[5];
  sprintf(a, "%x", out);
  if (strlen(a) % 2 != 0) {
    strcpy(buf, "0");
    strcat(buf, a);
  }else{
    strcpy(buf, a);
  }
}

void Packet::hexs(char *buf, unsigned int in) {
  sprintf(buf, "%x", in);
  if (in <= 15) {
    buf[1] = buf[0];
    buf[0] = '0';
  }
}

Packet* Packet::decode(char* raw) {
  Packet* out = new Packet();
  // CHECK START BYTE
  if (raw[0] != '#') {
    #ifndef ARDUINO
    throw 1;
    #else
    Serial.println("start");
    return NULL;
    #endif
  }
  // CHECK LENGTH
  char temp[3];
  temp[0] = raw[1];
  temp[1] = raw[2];
  temp[2] = '\0';
  int length = strtol(temp, NULL, 16);
  if (length != strlen(raw)) {
    #ifndef ARDUINO
    throw 2;
    #else
    Serial.println("length");
    return NULL;
    #endif
  }
  // CHECK END BYTE
  if (raw[length-1] != ';') {
    #ifndef ARDUINO
    throw 3;
    #else
    Serial.println("end");
    return NULL;
    #endif
  }
  // GET TYPE
  char typea[2];
  typea[0] = raw[3];
  typea[1] = raw[4];
  int type = strtol(typea, NULL, 16);
  out->set_type(static_cast<PacketType>(type));
  // CHECK CHECKSUM
  char old_checksum[5];
  memcpy( old_checksum, &raw[length - 5], 4 );
  old_checksum[4] = '\0';
  char new_checksum[245];
  memcpy(new_checksum, &raw[0], length - 5);
  new_checksum[length - 5] = '\0';
  char checksum[5];
  Packet::fletcher16(checksum, new_checksum);
  checksum[4] = '\0';
  if (strcmp(old_checksum, checksum)) {
    #ifndef ARDUINO
    throw 4;
    #else
    Serial.println("checksum");
    return NULL;
    #endif
  }
  // CONVERT DATA
  char data[245];
  memcpy(data, &raw[5], length - 10);
  data[length - 5] = '\0';
  out->set_data(data);
  return out;
}
