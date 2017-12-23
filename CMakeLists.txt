cmake_minimum_required(VERSION 3.0.0)


set(OTHER_FILES_EXTENSION "")

set (PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

      if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            message("Add define DEBUGGING")
            add_definitions(-DDEBUGGING)
      endif()

      # config here
      set(PROJECT_NAME "dsa171a2")
      set(TEST_NAME "dsa171a2-test")

      set(SOURCE_FOLDER "src")
      set(TEST_FOLDER "test")
      set(INCLUDE_FOLDER "include")
      set(OUTPUT_FOLDER "bin")

      # vs only settings
      set(OTHER_FILES_FOLDER "resource")

      set(DEBUG_FOLDER "bin") # vs only

# output file exe location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_FOLDER})

# the project name
project (${PROJECT_NAME} CXX)


# the included folder for <foo.h>
include_directories(${INCLUDE_FOLDER})
include_directories("${INCLUDE_FOLDER}/spdlog/include/")
include_directories("${INCLUDE_FOLDER}/googletest/googletest/include")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
# using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# using g++
set(WARNING "-Wall")
set(PUBLIC "-static -static-libgcc -static-libstdc++")

set(IDENTIFIER "-fextended-identifiers --input-charset=utf-8 -finput-charset=utf-8 --exec-charset=utf-8")

set(CPPVERSION "-std=c++11")
set(MULTITHREAD "-pthread")

set(CXX_DEFINES "-DAVL_USE_HEIGHT")

set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} \
      ${WARNING} \
      ${IDENTIFIER} \
      ${CPPVERSION} \
      ${MULTITHREAD} \
      ${CXX_DEFINES}"
)


add_subdirectory("src")

if (UNIT_TEST)
message("Add define UNIT_TEST")
add_definitions(-DUNIT_TEST)
add_subdirectory("include/googletest/googletest")
add_subdirectory("test")
endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

      # the included folder for <foo.h>
      include_directories(${INCLUDE_FOLDER})
      file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${SOURCE_FOLDER}/*.cpp")
      file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INCLUDE_FOLDER}/*.h" "${INCLUDE_FOLDER}/*.hpp")
      file(GLOB_RECURSE RESOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${OTHER_FILES_FOLDER}/*.${OTHER_FILES_EXTENSION}")

      set(ALL_FILES ${SOURCES}\  ${HEADERS}\  ${RESOURCE})
      add_executable(${PROJECT_NAME} ${ALL_FILES})



      # using filter in VS
      set_property(GLOBAL PROPERTY USE_FOLDERS ON)

      # tạo folder cho project
      foreach(FILE ${ALL_FILES})
            get_filename_component(PARENT_DIR "${FILE}" PATH)

            # skip src or include and changes /'s to \\'s
            string(REGEX REPLACE "(\\./)?(${SOURCE_FOLDER}|${INCLUDE_FOLDER}|${OTHER_FILES_FOLDER})/?" "" GROUP "${PARENT_DIR}")
            string(REPLACE "/" "\\" GROUP "${GROUP}")

            # group into "Source Files" and "Header Files"
            if ("${FILE}" MATCHES ".*\\.cpp")
                  set(GROUP "Source Files\\${GROUP}")
            elseif("${FILE}" MATCHES ".*\\.h")
                  set(GROUP "Header Files\\${GROUP}")
            elseif("${FILE}" MATCHES ".*\\.${OTHER_FILES_EXTENSION}")
                  set(GROUP "Resource Files\\${GROUP}")
            endif()

            source_group("${GROUP}" FILES "${FILE}")
      endforeach()

      # thư mục khi chạy Debug
      set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEBUG_FOLDER}")
endif()
