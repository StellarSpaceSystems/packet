from packet import Packet, DecodeException
import sys

if __name__ == '__main__':
    print(sys.argv)
    print(Packet.fletcher16(sys.argv[1]))
    """if sys.argv[1] == "e":
        p = Packet()
        p.type = Packet.Type.TELEMETRY
        p.data = sys.argv[2]
        print(p.encode())
    elif sys.argv[1] == "d":
        try:
            p = Packet.decode(sys.argv[2])
            print(p.type)
            print(p.data)
        except DecodeException as e:
            print(e)"""
