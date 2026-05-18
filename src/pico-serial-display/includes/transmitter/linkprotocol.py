import serial

ser = serial.Serial('/dev/ttyS3', 921600, timeout=1) 

def linkprotocol(link):
    if link == 0x00:
        print("Packet ACK")
        print("Next PKT")
        delmove("data.bin")
    elif link >= 0x01:
        print("Packet NACK")
        print("Resend PKT")

def await_response():
    if ser.in_waiting > 0:
        link = ser.read(1)[0]
        print("Response received!")
        linkprotocol(link)

    else:
        return

#packet sender
def send_packet(packet_hex_list):

    # hex string -> int
    int_list = [int(x, 16) for x in packet_hex_list]

    # int -> raw bytes
    packet = bytes(int_list)

    print("Sending:")
    print(packet_hex_list)

    ser.write(packet)
    ser.flush()

#reads data.bin
def readbin(filename):
    with open(filename, "rb") as file:
        line = file.readline()  
    return line

#deletes content from data.bin and moves all content up a line
def delmove(filename):
    with open(filename, "rb") as file:
        lines = file.readlines()

    lines = lines[1:]

    with open(filename, "wb") as file:
        file.writelines(lines)