#include "../packet.h"

#include <cstdio>

int main(int argc, char const *argv[]) {
	Packet* p2 = new Packet();
	p2->set_type(PacketType::TELEMETRY);
	p2->set_data("{\"state\":0,\"old_state\":0,\"mv1\":6431}");
	char buf[100];
	p2->encode(buf);
	printf("%s\n", buf);
	Packet* p = Packet::decode(buf);
	printf("%s\n", p->get_data());
	return 0;
}
