# Error-Detecting and Correcting Codes: Hamming & CRC Implementations

## Overview

This repository contains implementations of Hamming (7,4) and CRC-4 error-detecting and correcting codes for serial communication. The project demonstrates the application of these codes in a communication protocol where data is sent and received in a simulated environment.

The data used for transmission is a static string composed of names, `"Shaked & Priel"`. The project includes implementations for both Hamming and CRC codes in separate files, and it provides a framework for testing and verifying the correctness of the codes.

## Files

- **`hamming_code.INO`**: Implementation of Hamming (7,4) error-correcting code.
- **`crc_code.INO`**: Implementation of CRC-4 error-detecting code.
  
## Implementation Details

### 1. Communication Layers

The project uses a layered approach for communication:

- **Layer 1 (Physical Layer)**: Handles the transmission and reception of raw data bits. This layer is based on functions from Labs 1/2, such as `uart_tx()` and `usart_tx()`.
- **Layer 2 (Data Link Layer)**: Implements error-detecting and correcting codes. The functions here interact with Layer 1 to handle data transmission and reception, using flags to coordinate between layers.

### 2. Functions

#### Layer 2 Functions

- **`Hamming47_tx()`**: Splits an 8-bit character into two 4-bit nibbles, calculates the parity bits for each nibble, and prints the result (7 bits, in binary).
- **`Hamming47_rx()`**: Checks each nibble for errors, issues a fix if needed, and reassembles the original ASCII character. It prints the received word (7 bits) and the corrected word if an error was detected.

- **`CRC4_tx()`**: Reads an 8-bit character from the name string and calculates the CRC-4. It prints the result (12 bits, in binary).
- **`CRC4_rx()`**: Checks errors in the received character and CRC-4 bits. It prints the character along with CRC status (fail/success).

#### Layer 1 Functions

- **`layer1_tx()`**: Handles the transmission of data bits to the physical layer.
- **`layer1_rx()`**: Handles the reception of data bits from the physical layer.

### 3. Loop Function

The `loop()` function in the main code coordinates the operations between layers:

```cpp
void loop() {
    layer2_tx(); // Calls Hamming47_tx() or CRC4_tx()
    layer2_rx(); // Calls the Rx version of the above
    layer1_tx(); // Either uart_tx() or usart_tx() from Labs 1/2
```

### 4. Error Testing
* No Errors: Demonstrates successful transmission and reception without errors.
* One Bit Error: Shows the original transmitted word, the received word, and the corrected result.
* Two Bits Error: Similar to one-bit error test but with two bits changed.
* Three Bits Error: Tests the scenario with three bits changed and demonstrates the result.
    layer1_rx(); // Rx version of the above
}
