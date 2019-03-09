project(chomotkar)

cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${chomotkar_SOURCE_DIR})

ADD_LIBRARY(LibsModule 
        main.c 
        downloadHelper.c
    )

target_link_libraries(LibsModule -lcurl)

add_executable(chomotkar main.c downloadHelper.c)

target_link_libraries(chomotkar LibsModule)


install(
    TARGETS
        chomotkar
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)