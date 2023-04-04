cmake_minimum_required(VERSION 3.16.3)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig)
    message(FATAL_ERROR "sdkconfig not found (please run ./switchconf.sh)")
endif()

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)
    message(FATAL_ERROR "config.cmake not found (please run ./switchconf.sh)")
endif()

file(READ_SYMLINK ${CMAKE_CURRENT_SOURCE_DIR}/sdkconfig result)
if (result STREQUAL "")
    message(FATAL_ERROR "sdkconfig is not a symlink (please run ./switchconf.sh)")
endif()

file(READ_SYMLINK ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake result)
if (result STREQUAL "")
    message(FATAL_ERROR "config.cmake is not a symlink (please run ./switchconf.sh)")
endif()

include(config.cmake)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

idf_build_set_property(CXX_COMPILE_OPTIONS -std=gnu++23 APPEND)

set(EXTRA_COMPONENT_DIRS
    esp_boost
    esp-protocols/components
)

project(${BOBBY_APP_NAME})

set(expected_build_folder "${CMAKE_CURRENT_SOURCE_DIR}/build")
file(READ_SYMLINK "${expected_build_folder}" expected_build_folder_resolved)
if(NOT IS_ABSOLUTE "${expected_build_folder_resolved}")
  get_filename_component(dir "${expected_build_folder}" DIRECTORY)
  set(expected_build_folder_resolved "${dir}/${expected_build_folder_resolved}")
endif()

if (NOT PROJECT_BINARY_DIR STREQUAL "${expected_build_folder}" AND NOT PROJECT_BINARY_DIR STREQUAL "${expected_build_folder_resolved}")
    message(FATAL_ERROR "building in ${PROJECT_BINARY_DIR} is not supported, you have to build in ${expected_build_folder} or ${expected_build_folder_resolved} (please run ./switchconf.sh)")
endif()
