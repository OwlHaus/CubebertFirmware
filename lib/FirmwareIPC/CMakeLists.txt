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

target_link_libraries(
  ${LIB_NAME}
  pico_stdlib
  hardware_i2c
)

add_dependencies(
  ${LIB_NAME}
  pico_stdlib
  hardware_i2c
)

install(TARGETS ${LIB_NAME} DESTINATION lib)
