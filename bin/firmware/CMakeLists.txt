set(BIN_NAME CubebertFirmware)

add_executable(${BIN_NAME} cubebert-firmware.cpp state.cpp)

target_compile_features(${BIN_NAME} PUBLIC cxx_std_17)

target_link_libraries(
  ${BIN_NAME}
  PUBLIC pico_stdlib
  PUBLIC ClawControl
  PUBLIC FirmwareIPC
  PUBLIC Lighting
)

pico_set_program_name(${BIN_NAME} "${BIN_NAME}")
pico_set_program_version(${BIN_NAME} "0.1")

pico_enable_stdio_usb(${BIN_NAME} 1)
pico_enable_stdio_uart(${BIN_NAME} 0)

add_dependencies(
  ${BIN_NAME}
  pico_stdlib
)

pico_add_extra_outputs(${BIN_NAME})

# set output in build directory to top level instead of bin/
set_target_properties(${BIN_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
