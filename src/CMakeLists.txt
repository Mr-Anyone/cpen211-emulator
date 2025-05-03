include(FetchContent)

FetchContent_Declare(
    elfio
    GIT_REPOSITORY  https://github.com/serge1/ELFIO.git
    GIT_TAG         7a1de75522f0d1e42a68d206c8165b45327ac16e
)

FetchContent_MakeAvailable(elfio)

add_executable(
    cpen211-emulator

    main.cpp
    parser.cpp
    machine.cpp
)

target_include_directories(cpen211-emulator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(cpen211-emulator elfio)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_programs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# install program
 install(TARGETS cpen211-emulator DESTINATION bin)
