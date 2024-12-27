# Zephyr DMA Memory Copy Example for ESP32-S3

This project demonstrates how to use Direct Memory Access (DMA) to copy data from one memory location to another using Zephyr RTOS and the DMA API. The initial implementation focuses on copying data within the internal RAM. Future updates will include support for copying data to external memories such as PSRAM.

## Features
- Demonstrates basic DMA functionality and configuration.
- Uses Zephyr’s DMA API for portability and ease of integration.
- Simple example of memory-to-memory data transfer within internal RAM.
- Structured for extensibility to support external PSRAM in future updates.

## Requirements
- A zephyr supported boarch such as ESP32-S3.
- Zephyr development environment set up.

## Getting Started

### Prerequisites
Ensure you have the following tools installed after following the [Zephyr Getting Started tutorial](https://docs.zephyrproject.org/latest/develop/getting_started/index.html):
- Zephyr SDK.
- Device Tree compiler
- CMake and Ninja build system.
- Python (for west command-line tool).

### Setup
Clone this repository:
   ```sh
   git clone https://github.com/jeronimoagullo/Zephyr-DMA-Sample.git
   ```

### Building and Flashing
This project was tested with esp32s3 devkit board:

1. Build the project:
   ```sh
   west build -b esp32s3_devkitm/esp32s3/procpu
   ```
2. Flash the firmware to the ESP32-S3:
   ```sh
   west flash
   ```

### Expected output
This app generates the following output for a buffer of 10 bytes:

```
*** Booting Zephyr OS build 4488ed1a20a9 ***
[00:00:00.218,000] <inf> main: buffer allocated at: 0x3fc95498
[00:00:00.218,000] <inf> main: buffer allocated at: 0x3fc954b0
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [0] 0x3fc95498
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [1] 0x3fc95499
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [2] 0x3fc9549a
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [3] 0x3fc9549b
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [4] 0x3fc9549c
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [5] 0x3fc9549d
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [6] 0x3fc9549e
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [7] 0x3fc9549f
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [8] 0x3fc954a0
[00:00:00.218,000] <dbg> main: fill_buffer_with_random_content: [9] 0x3fc954a1
[00:00:00.218,000] <inf> main: DMA device is ready
[00:00:00.218,000] <dbg> main: configure_dma_transfer: source: 0x3fc95498, dest: 0x3fc954b0
[00:00:00.218,000] <inf> main: Configuring DMA...
[00:00:00.218,000] <inf> main: DMA configured
[00:00:00.218,000] <inf> main: DMA started
[00:00:00.218,000] <inf> main: DMA transfer complete on channel 3
[00:00:00.218,000] <inf> main: DMA transfer complete on channel 2
[00:00:00.318,000] <dbg> main: verify_buffer_content: verify_buffer_content:
[00:00:00.318,000] <dbg> main: verify_buffer_content: 0 => [0x3fc95498] 198 - [0x3fc954b0] 198
[00:00:00.318,000] <dbg> main: verify_buffer_content: 1 => [0x3fc95499] 90 - [0x3fc954b1] 90
[00:00:00.318,000] <dbg> main: verify_buffer_content: 2 => [0x3fc9549a] 63 - [0x3fc954b2] 63
[00:00:00.318,000] <dbg> main: verify_buffer_content: 3 => [0x3fc9549b] 189 - [0x3fc954b3] 189
[00:00:00.318,000] <dbg> main: verify_buffer_content: 4 => [0x3fc9549c] 4 - [0x3fc954b4] 4
[00:00:00.318,000] <dbg> main: verify_buffer_content: 5 => [0x3fc9549d] 115 - [0x3fc954b5] 115
[00:00:00.318,000] <dbg> main: verify_buffer_content: 6 => [0x3fc9549e] 136 - [0x3fc954b6] 136
[00:00:00.318,000] <dbg> main: verify_buffer_content: 7 => [0x3fc9549f] 133 - [0x3fc954b7] 133
[00:00:00.318,000] <dbg> main: verify_buffer_content: 8 => [0x3fc954a0] 13 - [0x3fc954b8] 13
[00:00:00.318,000] <dbg> main: verify_buffer_content: 9 => [0x3fc954a1] 88 - [0x3fc954b9] 88
[00:00:00.318,000] <inf> main: Buffer content matches
```

## Code Structure
- **`src/main.c`**: Contains the main logic for DMA initialization and memory-to-memory transfer.
- **`prj.conf`**: Zephyr configuration file enabling DMA drivers.
- **`CMakeLists.txt`**: Build system configuration.

## Example Walkthrough

The example demonstrates the following steps:
1. **DMA Initialization**:
   - Configures the DMA controller with appropriate channels and descriptors.
2. **Memory Allocation**:
   - Allocates source and destination buffers in internal RAM.
3. **Data Copy**:
   - Initiates a DMA transfer to copy data from the source buffer to the destination buffer.
   - Verifies data integrity by comparing the source and destination buffers.

### Key Functions
- **`dma_config()`**: Configures the DMA channel and settings.
- **`dma_start()`**: Initiates the DMA transfer.
- **`verify_data()`**: Verifies the integrity of the transferred data.

## Future Work
- Add support for DMA transfers to/from external PSRAM.
- Implement more complex DMA operations, such as scatter-gather transfers.
- Provide benchmarks and performance metrics for different memory configurations.

## References
- [Zephyr Documentation](https://docs.zephyrproject.org/latest/index.html)
- [ESP32-S3 Technical Reference Manual](https://www.espressif.com/en/support/documents/technical-documents)
- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)

## Contributing
Contributions are welcome! Please submit a pull request or open an issue to suggest improvements or report bugs.

## License
This project is licensed under the MIT License. See the LICENSE file for details.

