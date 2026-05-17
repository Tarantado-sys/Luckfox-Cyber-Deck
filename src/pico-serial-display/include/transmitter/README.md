# Serial Packet Protocol Communication

A Python-based serial communication system for sending structured packets with error detection and automatic retransmission capabilities.

## Overview

This project implements a custom binary protocol for communicating over serial connections with the following features:

- **Packet Structuring**: Organized binary packets with command, sequence, length, and parameters
- **Error Detection**: CRC16-CCITT checksum calculation for data integrity
- **Frame Formatting**: Special byte escaping and Start-of-Frame (SOF) markers for robust packet delimitation
- **Response Handling**: Automatic ACK/NACK packet management with retry queue support
- **Queue Management**: Persistent binary file storage for packet retransmission

## How It Works

### Packet Structure

Each packet consists of:
1. **Start of Frame** (SOF): Start of Packet Identifier 
2. **Sequence Number** (seq): Packet identifier
3. **Length** (length): Number of parameter bytes
4. **Command** (cmd): Command code indicating the action
5. **Parameters** (params): Variable-length payload data
6. **Checksum**: CRC16-CCITT validation bytes (high and low)

### Packet Building Process

**Step 1: Packet Assembly**
```
Raw packet = [seq, length, cmd] + params
```

**Step 2: Checksum Calculation**
```
CRC = CRC16-CCITT(raw packet)
crc_high = (CRC >> 8) & 0xFF
crc_low = CRC & 0xFF
```

**Step 3: Frame Formatting**
```
Complete packet = [seq, length, cmd] + params + [crc_high, crc_low]
```

Special bytes (0xAA, 0x55, 0x7D) are escaped with prefix 0x7D and XOR'd with 0x20.

**Step 4: Framing**
```
Framed packet = [0xAA, 0x55] + escaped_packet
```

The SOF (Start-of-Frame) markers [0xAA, 0x55] signal the beginning of each transmission.

### Serial Communication Flow

1. **Packet Transmission**
   - Converts hex packet to bytes
   - Sends via serial at 921600 baud
   - Flushes the buffer to ensure delivery

2. **Response Handling**
   - Listens for ACK (0x00) or NACK (≥ 0x01) responses
   - ACK: Removes sent packet from queue, ready for next transmission
   - NACK: Keeps packet in queue for automatic retransmission

3. **Queue Management**
   - Packets stored in `data.bin` with newline delimiters
   - Failed packets persist for retry
   - Successful packets are removed and next packet sent

### CRC Algorithm

Uses CRC16-CCITT polynomial (0x1021) with initial value 0xFFFF:
- Processes 8 bits per byte
- XOR operation on MSB detection
- Maintains 16-bit boundary after each shift

## Project Components

### `packetbuilder.py`
- `crc16_ccitt()`: Calculates CRC16-CCITT checksum
- `packetadder()`: Assembles raw packet structure
- `packetrestructure()`: Adds checksum to packet
- `framer()`: Applies SOF and byte escaping
- `append_bytes()`: Stores packets in binary file

### `linkprotocol.py`
- `send_packet()`: Transmits framed packet over serial
- `await_response()`: Monitors for device responses
- `linkprotocol()`: Processes ACK/NACK and manages retransmission
- `readbin()`: Retrieves packets from queue
- `delmove()`: Removes acknowledged packets from queue

### `main.py`
- Orchestrates the complete packet lifecycle
- Creates sample packets for testing
- Manages continuous communication loop

## Serial Configuration

- **Port**: `/dev/ttyS3`
- **Baud Rate**: 921600
- **Timeout**: 1 second

> [!NOTE]
> I haven`t implemented the full packet queue system.

## Changelog
- Changed the `CRC algorithm` to better fit the `CRC algorithm` for the reciever written in C++.