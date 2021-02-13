const Type = {
  HEARTBEAT: 0x01,
  TELEMETRY: 0x02,
  IGNITION: 0x03,
  SHUTDOWN: 0x04,
  ACK: 0x05,
  CONF: 0x06,
  INFO: 0x07
}

class Packet {
  constructor() {
    this._type = null
    this.data = ""
  }

  length() {
    return (10 + this.data.length).toString(16).padStart(2, '0')
  }

  set type(type) {
    this._type = type
  }

  get type() {
    return this._type
  }

  set data(data) {
    this._data = data
  }

  get data() {
    return this._data
  }

  encode() {
    var chunk1 = '#' + this.length() + this.type.toString(16).padStart(2, '0') + this.data;
    return chunk1 + this.constructor.fletcher16(chunk1).toString(16).padStart(4, '0') + ';'
  }

  static fletcher16(data) {
    var buf = Buffer.from(data, 'utf8')
    var sum1 = 0xff, sum2 = 0xff;
    var i = 0;
    var len = buf.length;

    while (len) {
      var tlen = len > 20 ? 20 : len;
      len -= tlen;
      do {
        sum2 += sum1 += buf[i++];
      } while (--tlen);
      sum1 = (sum1 & 0xff) + (sum1 >> 8);
      sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }
    /* Second reduction step to reduce sums to 8 bits */
    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;
  }

  static decode(data) {
    if (data.charAt(0) != '#') {
      throw "Start byte not present, first byte is " + data.charAt(0)
    }
    var length = parseInt(data.charAt(1) + data.charAt(2), 16)
    if (length != data.length) {
      throw "Length does not match, expects " + length + ", got " + data.length
    }
    if (data.charAt(data.length - 1) != ';') {
      throw "End byte not present, last byte is " + data.charAt(data.length - 1)
    }
    if (Object.keys(Type).find(
      key => Type[key] === parseInt(data.charAt(3) + data.charAt(4), 16)
    ) === undefined) {
      throw "Unrecognized packet type: " + parseInt(data.charAt(3) + data.charAt(4), 16)
    }
    var old_checksum = data.slice(length - 5, length - 1)
    var new_checksum = this.fletcher16(data.slice(0, length - 5)).toString(16).padStart(4, '0')
    if (old_checksum != new_checksum) {
      throw "Checksum does not match, expects " + new_checksum + ", got " + old_checksum
    }
    var out = new Packet()
    out.type = parseInt(data.charAt(3) + data.charAt(4), 16)
    out.data = data.slice(5, length - 5)
    return out
  }
}

module.exports = Packet;
module.exports.Type = Type;
