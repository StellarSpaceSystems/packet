Packet = require('./packet')

p = new Packet()
p.type = Packet.Type.TELEMETRY
p.data = "{\"value\": 0, \"value2\": 0.0}"
d = p.encode()
console.log(d);

console.log(Packet.decode(d));

console.log(Packet.fletcher16('#2502{"value": 0, "value2": 0.0}').toString(16));
