set(BIN_NAME cubebert-firmware)

add_executable(${BIN_NAME} cubebert-firmware.cpp state.cpp)

target_compile_features(${BIN_NAME} PUBLIC cxx_std_11)

target_link_libraries(
  ${BIN_NAME}
  PUBLIC pico_stdlib
  PUBLIC ClawControl
  PUBLIC FirmwareIPC
  PUBLIC Lighting
)

pico_enable_stdio_usb(${BIN_NAME} 1)
pico_enable_stdio_uart(${BIN_NAME} 0)

add_dependencies(
  ${BIN_NAME}
  pico_stdlib
)

install(TARGETS ${BIN_NAME} DESTINATION sbin)

pico_add_extra_outputs(${BIN_NAME})
