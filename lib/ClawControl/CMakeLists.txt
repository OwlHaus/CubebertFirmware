set(LIB_NAME ClawControl)

add_library(${LIB_NAME} claw.cpp grip.cpp rcsp.cpp wrist.cpp)

pico_generate_pio_header(${LIB_NAME} ${CMAKE_CURRENT_LIST_DIR}/stepper.pio)

include(FetchContent)
FetchContent_Declare(
  Servo
  GIT_REPOSITORY https://github.com/Danieo6/Servo
  GIT_TAG master
)
FetchContent_MakeAvailable(Servo)

target_include_directories(
  ${LIB_NAME}
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${servo_SOURCE_DIR}/src
)

target_link_libraries(
  ${LIB_NAME}
  PUBLIC FirmwareIPC
  pico_stdlib
  hardware_pio
  hardware_pwm
  Servo
)

add_dependencies(
  ${LIB_NAME}
  pico_stdlib
  hardware_pio
  hardware_pwm
)

install(TARGETS ${LIB_NAME} DESTINATION lib)
