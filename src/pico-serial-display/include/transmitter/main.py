from packetbuilder import packetadder
from packetbuilder import packetrestructure
from packetbuilder import framer
from packetbuilder import append_bytes
from linkprotocol import await_response
from linkprotocol import send_packet
from linkprotocol import readbin

# example variables 
# must change to input
cmd = 0x0F
seq = 0x01
length = 0x04
params = [0x14, 0x64, 0xC8, 0x7F]

#crc algo
class CRC16:
    def __init__(self, polynomial=0xA001, init=0xFFFF, xor_out=0x0000):
        self.polynomial = polynomial
        self.init = init
        self.xor_out = xor_out

        self.mask = 0xFFFF
        self.table = self._make_table()
        self.reset()

    def reset(self):
        self.crc = self.init

    def _make_table(self):
        table = []
        for i in range(256):
            crc = i
            for _ in range(8):
                if crc & 1:
                    crc = (crc >> 1) ^ self.polynomial
                else:
                    crc >>= 1
            table.append(crc & 0xFFFF)
        return table

    def update(self, data: bytes):
        for b in data:
            self.crc = self.table[(self.crc ^ b) & 0xFF] ^ (self.crc >> 8)
            self.crc &= self.mask

    def digest(self):
        return self.crc ^ self.xor_out

    def hexdigest(self):
        return f"{self.digest():04X}"

packet = packetadder(seq, length, cmd, params)
data = bytes(packet)
crc = CRC16()
crc.update(data)
value = crc.digest()  # 👈 get integer CRC result

print(hex(value))
print(f"{value:04X}")

crc_high = (value >> 8) & 0xFF
crc_low  = value & 0xFF

#packet restructure (no sof, eof or esc)
pktres = packetrestructure(seq, length, cmd, params, crc_high, crc_low) #raw out
pkthex = [hex(x) for x in pktres] #convert raw to hex

#packet restructure (with sof and esc)
pkt = [seq, length, cmd,] + params + [crc_high, crc_low]
framed = framer(pkt) #raw out
pkthex1 = [hex(x) for x in framed] #convert raw to hex

#raw out
print("unframed packet in int/raw form:", pktres)
print("framed packet in int/raw form:", framed)
# #out (converted to hex) for output only
print("unframed packet in hex:", pkthex)
print("framed packet in hex:", pkthex1)

append_bytes("data.bin", pkthex1)

datahex = readbin("data.bin")
datahex1 = [hex(x) for x in datahex]

send_packet(datahex1)

while True:
    await_response()