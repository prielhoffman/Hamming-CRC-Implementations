# Hamming and CRC Communication Codes

An **Arduino/C++ implementation of Hamming (7,4) and CRC-4** for serial communication, demonstrating how error-detecting and error-correcting codes can be integrated into a simple layered communication system.

This project focuses on data-link-layer reliability concepts and shows how encoded data can be transmitted, checked, corrected, and reconstructed in an embedded communication environment.

## Tech Stack

- **Language:** C++
- **Platform:** Arduino
- **Concepts:** Hamming code, CRC, serial communication, error detection, error correction, embedded systems, layered communication

## What the Project Does

The project implements two classic reliability mechanisms used in communication systems:

- **Hamming (7,4)** for single-bit error correction
- **CRC-4** for error detection

Both implementations operate within a layered communication model, where:

- **Layer 1** handles raw bit transmission and reception
- **Layer 2** applies the coding logic for error detection and correction

The project demonstrates how encoded data is generated, transmitted, received, checked for errors, and either corrected or validated depending on the coding method used. :contentReference[oaicite:1]{index=1}

## Main Features

- Hamming (7,4) encoding and decoding
- CRC-4 generation and validation
- layered communication structure
- serial transmission and reception flow
- single-bit error correction with Hamming
- error detection with CRC
- testing under multiple error scenarios
- embedded implementation in Arduino C++

## Project Structure

**Files:**
- `hamming_code.INO` – Hamming (7,4) implementation
- `crc_code.INO` – CRC-4 implementation :contentReference[oaicite:2]{index=2}

## Communication Model

The project is organized using a simple layered design.

### Layer 1 – Physical Layer

This layer is responsible for transmitting and receiving raw bits.

It is based on low-level communication functions such as:
- `uart_tx()`
- `usart_tx()`

Layer 1 handles the physical movement of bits between sender and receiver. :contentReference[oaicite:3]{index=3}

### Layer 2 – Data Link Layer

This layer implements the coding logic used to improve communication reliability.

Depending on the selected mode, it performs:
- Hamming encoding / decoding
- CRC generation / checking
- interaction with Layer 1 for actual transmission and reception

This separation makes the project easier to reason about and reflects a simplified communication stack. :contentReference[oaicite:4]{index=4}

## Hamming (7,4) Implementation

The Hamming portion of the project takes each 8-bit character, splits it into two 4-bit nibbles, and encodes each nibble using Hamming (7,4).

### Transmit Flow

- split character into two 4-bit parts
- calculate parity bits
- construct Hamming code words
- transmit the encoded bits

### Receive Flow

- receive encoded words
- calculate syndrome
- detect and correct a single-bit error when possible
- reconstruct the original ASCII character

This part of the project demonstrates practical error correction at the bit level. :contentReference[oaicite:5]{index=5}

## CRC-4 Implementation

The CRC portion of the project generates a 4-bit cyclic redundancy check for transmitted data.

### Transmit Flow

- read an 8-bit character
- compute CRC-4 bits
- append CRC to the transmitted data

### Receive Flow

- receive the character and CRC bits
- recompute the CRC
- compare expected and received values
- report whether validation passed or failed

Unlike Hamming, CRC is used here for **error detection**, not correction. :contentReference[oaicite:6]{index=6}

## Key Functions

### Layer 2 Functions

- `Hamming47_tx()` – encodes data using Hamming (7,4)
- `Hamming47_rx()` – checks received Hamming words, corrects errors when possible, and reconstructs the original character
- `CRC4_tx()` – computes CRC-4 for transmitted data
- `CRC4_rx()` – validates received data using CRC-4

### Layer 1 Functions

- `layer1_tx()` – passes data to the physical transmission layer
- `layer1_rx()` – receives raw bits from the physical layer :contentReference[oaicite:7]{index=7}

## Typical Execution Flow

The communication loop coordinates the interaction between the layers:

```cpp
void loop() {
    layer2_tx();
    layer2_rx();
    layer1_tx();
    layer1_rx();
}
```

This structure highlights the relationship between coding logic and physical transmission. :contentReference[oaicite:8]{index=8}

## Error Testing

The project includes tests for multiple transmission scenarios:

- no error
- one-bit error
- two-bit error
- three-bit error

These cases help demonstrate the difference between:

- what Hamming can correct
- what CRC can detect
- how reliability mechanisms behave under different corruption levels :contentReference[oaicite:9]{index=9}

## What I Implemented

This project focused on applying communication theory in code and connecting reliability concepts to actual transmission flow.

Key implementation areas included:

- Hamming (7,4) encoding and decoding logic
- CRC-4 generation and validation
- integration between data-link and physical-layer functions
- serial communication flow between transmit and receive paths
- error testing for different corruption scenarios
- embedded implementation in Arduino C++

## Why This Project Matters

This project demonstrates practical understanding of:

- error detection vs. error correction
- Hamming codes
- cyclic redundancy checks
- layered communication design
- serial communication in embedded systems
- applying communication theory in real code

It is a strong example of low-level communication work because it connects theory, protocol logic, and implementation in one project.

## Core Concepts Practiced

- Hamming (7,4)
- CRC-4
- serial communication
- error detection
- error correction
- bit-level data handling
- layered system design
- embedded systems programming

## Key Takeaways

Through this project, I strengthened my understanding of:

- how communication systems detect and correct transmission errors
- the difference between correction-based and detection-based approaches
- how coding logic can be integrated into a layered communication model
- how to implement communication theory concepts on embedded hardware
- how bit-level reliability mechanisms affect received data integrity

## Future Improvements

Possible next steps for the project:

- add clearer visualization of encoded and decoded bit flows
- compare Hamming and CRC behavior quantitatively across more test cases
- support configurable input data instead of a fixed static string
- extend the project with additional coding schemes
- document the hardware setup and serial testing workflow
- add structured logging for transmission, reception, and error events
