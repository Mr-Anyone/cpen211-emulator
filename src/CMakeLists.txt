add_executable(
    emulator

    main.cpp
    parser.cpp
    machine.cpp
)

target_include_directories(emulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_programs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
