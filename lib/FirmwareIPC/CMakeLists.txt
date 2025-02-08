set(LIB_NAME FirmwareIPC)

add_library(
  ${LIB_NAME}
  command.cpp
  i2c.cpp
  uart.cpp
)

target_include_directories(
  ${LIB_NAME}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

pico_enable_stdio_usb(${LIB_NAME} 1)
pico_enable_stdio_uart(${LIB_NAME} 0)

target_link_libraries(
  ${LIB_NAME}
  pico_stdlib
)

add_dependencies(
  ${LIB_NAME}
  pico_stdlib
)

install(TARGETS ${LIB_NAME} DESTINATION lib)
