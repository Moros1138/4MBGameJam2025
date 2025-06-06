cmake_minimum_required(VERSION 3.10)

project(4MB_Game_Jam_2025)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(raylib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} raylib)

if (${PLATFORM} STREQUAL "Web")

    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
    target_link_options(${PROJECT_NAME} PRIVATE --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@assets)
    
else()
    
    # Copy assets when not Web platform
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/assets
    )

endif()
