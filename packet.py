from enum import IntEnum

class Packet():
    class Type(IntEnum):
        HEARTBEAT = 0x01
        TELEMETRY = 0x02
        IGNITION = 0x03
        SHUTDOWN = 0x04
        ACK = 0x05
        CONF = 0x06
        INFO = 0x07

    def __init__(self):
        self.type = ""
        self.data = ""

    @property
    def length(self):
        return self.hexs(10 + len(self.data))

    @property
    def type(self):
        return self._type

    @type.setter
    def type(self, type):
        self._type = type

    @property
    def data(self):
        return self._data

    @data.setter
    def data(self, data):
        self._data = data

    def encode(self):
        chunk1 = '#' + self.length + self.hexs(self.type.value) + self.data
        checksum = self.fletcher16(chunk1)
        return chunk1 + checksum + ';'

    @staticmethod
    def fletcher16(bytestring):
        c0 = 0xff
        c1 = 0xff
        """
        modulus = 255
        bitshift = 8
        bytes_read = 1

        if type(bytestring) != bytes:
            bytestring = str.encode(bytestring)
        bytestring = bytearray(bytestring)

        # an efficient way of doing math.ceil() without frills
        iterations = ((len(bytestring) - 1) // bytes_read) + 1

        # actually updating the checksum by iterating through the bytes
        for i in range(0, iterations):
            start = bytes_read*i
            end = bytes_read*(i+1)

            bytepart = int.from_bytes(bytestring[start:end], byteorder="little")
            c0 = (c0 + bytepart) % modulus
            c1 = (c1 + c0) % modulus

        return hex((c1 << bitshift) + (c0))[2:].zfill(4)
        """
        bytestring = bytearray(str.encode(bytestring))
        i = 0
        length = len(bytestring)
        while length:
            tlen = 20 if length > 20 else length
            length -= tlen
            do = True
            while do:
                c0 += bytestring[i]
                i += 1
                c1 += c0
                tlen -= 1
                do = tlen
            c0 = (c0 & 0xff) + (c0 >> 8)
            c1 = (c1 & 0xff) + (c1 >> 8)
        c0 = (c0 & 0xff) + (c0 >> 8)
        c1 = (c1 & 0xff) + (c1 >> 8)
        return hex(c1 << 8 | c0)[2:].zfill(4)

    @staticmethod
    def decode(raw):
        start = raw[0]
        valid = True
        if start != '#':
            raise DecodeException("Start byte not present, first byte is " + start)
        length = int(raw[1] + raw[2], 16)
        if len(raw) != length:
            raise DecodeException("Length does not match, expects " + str(length) + ", got " + str(len(raw)))
        if raw[len(raw) - 1] != ';':
            raise DecodeException("End byte not present, last byte is " + raw[len(raw) - 1])
        type = int(raw[3] + raw[4], 16)
        checksum = raw[length - 5:length - 1]
        new_checksum = Packet.fletcher16(raw[0:length - 5])
        if checksum != new_checksum:
            raise DecodeException("Checksum does not match, expects " + new_checksum + ", got " + checksum)
        data = "".join(raw[5:length - 5])
        out = Packet()
        try:
            out.type = Packet.Type(type)
        except ValueError as e:
            raise DecodeException("Unrecognized packet type: " + hex(type))
        out.data = data
        return out

    @staticmethod
    def hexs(s):
        h = hex(s)
        if len(h) % 2 == 0:
            return h[2:]
        return '0' + h[2:]

class DecodeException(Exception):
    pass
