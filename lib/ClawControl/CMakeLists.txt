set(LIB_NAME ClawControl)

add_library(${LIB_NAME} claw.cpp fingers.cpp palm.cpp rcsp.cpp wrist.cpp)

pico_generate_pio_header(${LIB_NAME} ${CMAKE_CURRENT_LIST_DIR}/stepper.pio)

target_include_directories(
  ${LIB_NAME}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  ${LIB_NAME}
  PUBLIC FirmwareIPC
  pico_stdlib
  hardware_pio
  hardware_pwm
)

add_dependencies(
  ${LIB_NAME}
  pico_stdlib
  hardware_pio
  hardware_pwm
)

install(TARGETS ${LIB_NAME} DESTINATION lib)
