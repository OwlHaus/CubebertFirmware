# CubebertFirmware
Firmware Repo for Cubebert, imported as ExternalProject from Cubebert
Note: Will only bother to test compiling this on an RPI

Under construction, being rewritten for Micropython

## Getting Started

### Prerequisites
- VSCode
- VSCode Extension > Raspberry Pi Pico
- CMake > 3.13

### Recommended Extensions
- C/C++ by Microsoft
- C/C++ Extension Pack by Microsoft
- C/C++ Themes by Microsoft
- CMake Tools by Microsoft

### Building
You have two options for building

#### Extension Based
- Click the `Raspberry Pi Pico Project` Extension in the toolbar
- Select `Clean CMake`
- Select `Configure CMake`
- Boot RPi Pico into `BOOTSEL` Mode
- Select `Run Project (USB)`

#### Manual
```sh
# Configure
mkdir build
cd build
cmake ..

# Build
make

# Flash using picotool
```

## License

This project is licensed under the MIT [License](LICENSE)
