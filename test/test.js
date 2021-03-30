Packet = require('../packet')

p = new Packet()
p.type = Packet.Type.INFO
//p.data = "{\"value\": 0, \"value2\": 0.0}"
d = p.encode()
console.log(d);

console.log(Packet.decode(d));

console.log(Packet.fletcher16('#2f02{"state":0,"old_state":0,"mv1":-2299}').toString(16));
