# Packet

### Base Format

|Start Byte|Length|Packet Type|Data|Checksum|End Byte|
|-----------|-------|----|--|--|--|
|#|2 Bytes - hex int|2 Bytes - hex int|Variable Length|Fletcher 16 - four chars|;|


### Types

|Byte|Description|Data Format|Direction|
|-|-|-|-|
|0x01|Heartbeat|none|Either|
|0x02|Telemetry|JSON String|E > C|
|0x03|Ignition Command|Optional Delay (int, ms)|C > E|
|0x04|Shutdown Command|Optional Delay (int, ms)|C > E|
|0x05|ACK (Not used for Heartbeat/Telemetry)|Checksum of ACKed Packet|E > C|
|0x06|Conf|C > E: none (req), E > C: JSON String|Either|
|0x07|Info|C > E: none (req), E > C: JSON String|Either|

#### C++ Lib exception codes

|Type|Description|
|-|-|
|1|Invalid start byte|
|2|Invalid length|
|3|Invalid end byte|
|4|Invalid checksum|
