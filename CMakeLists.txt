PROJECT(chain-server)
cmake_minimum_required (VERSION 3.2)
list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake/macros")
include(AutoCollect)
set(CMAKE_CXX_STANDARD 17)
# cd build
# cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_CXX_FLAGS="-static"
PickIncludeDirectories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    INCLUDEDIRS
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/build)

PickSourceFiles(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    CODEFILES
    # Exclude
    ${CMAKE_CURRENT_SOURCE_DIR}/build)

PickIncludeDirectories(
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/chain-net/code/chain-net
        INCLUDEDIRS
        # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/build)

PickSourceFiles(
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/chain-net/code/chain-net
        CODEFILES
        # Exclude
        ${CMAKE_CURRENT_SOURCE_DIR}/build)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    PickIncludeDirectories(
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/mingw64/include
            INCLUDEDIRS
            # Exclude
            ${CMAKE_CURRENT_SOURCE_DIR}/build)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    PickIncludeDirectories(
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/linux/include
            INCLUDEDIRS
            # Exclude
            ${CMAKE_CURRENT_SOURCE_DIR}/build)
ELSE()
    PickIncludeDirectories(
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/bsd/include
            INCLUDEDIRS
            # Exclude
            ${CMAKE_CURRENT_SOURCE_DIR}/build)
ENDIF()

MakeFilter(
    ${CMAKE_CURRENT_SOURCE_DIR}/code)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    #set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
ENDIF()
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
ADD_EXECUTABLE(chain-server
    ${CODEFILES})
target_include_directories(chain-server
        PUBLIC ${INCLUDEDIRS})

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(chain-server pthread ws2_32
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/mingw64/lib/libflatbuffers.a)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(chain-server pthread
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/linux/lib/libflatbuffers.a)
ELSE()
    target_link_libraries(chain-server pthread
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/flatbuffers/bsd/lib/libflatbuffers.a)
ENDIF()


