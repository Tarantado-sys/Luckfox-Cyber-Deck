# simple crc algo
def crc16_ccitt(data: bytes, poly=0x1021, init_crc=0xFFFF):
    crc = init_crc

    for byte in data:
        crc ^= (byte << 8)  #bring byte into upper 8 bits

        for _ in range(8):  # process 8 bits
            if crc & 0x8000:  # if MSB is 1
                crc = (crc << 1) ^ poly
            else:
                crc = crc << 1

            crc &= 0xFFFF  # keep 16bit

    return crc

# add packets to list
def packetadder(cmd, seq, length, params):
    return [cmd, seq, length] + params

# restructures the packet (or adds the checksum to the packet) no sof and eof
def packetrestructure(seq, length, cmd,  params, crc_high, crc_low):
    fullpkt = [seq, length, cmd] + params + [crc_high, crc_low]
    return fullpkt

#DO NOT USE!! this is a prototype version!

# def framer(packet):
#     SOF = 0x7E
#     EOF = 0x7F
#     ESC = 0x7D

#     out = [SOF]

#     for b in packet:
#         if b in (SOF, EOF, ESC):
#             out.append(ESC)
#             out.append(b ^ 0x20)
#         else:
#             out.append(b)

#     out.append(EOF)
#     return out

# new packet framer SOF has 2 bytes no EOF
def framer(packet):
    SOF = [0xAA, 0x55]
    ESC = 0x7D

    out = []

    #add SOF bytes first
    out.extend(SOF)

    for b in packet:
        if b in (0xAA, 0x55, ESC):
            out.append(ESC)
            out.append(b ^ 0x20)
        else:
            out.append(b)

    return out


def append_bytes(filename, data):
    byte_data = bytes(int(x, 16) for x in data)

    with open(filename, "ab") as file:
        file.write(byte_data)
        file.write(b"\n")
