cmake_minimum_required(VERSION 3.22)
project(RemoteSignalSlot)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(fmt)
include_directories(include fmt/include)

add_library(RemoteSignalSlot src/utils.cpp src/SlotEnclosure.cpp src/ReflexManager.cpp)
target_link_libraries(RemoteSignalSlot fmt::fmt)

add_executable(testSignalSlot test/main.cpp)
target_link_libraries(testSignalSlot RemoteSignalSlot)


add_executable(testGenerateID test/testGenerateID.cpp )
target_link_libraries(testGenerateID RemoteSignalSlot)

add_executable(testTypeid test/testTypeid.cpp)
target_link_libraries(testTypeid RemoteSignalSlot)