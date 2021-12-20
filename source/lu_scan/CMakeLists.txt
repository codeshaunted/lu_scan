# averysumner - lu_scan
# source/lu_scan/CMakeLists.txt
# project source CMake file

set(LU_SCAN_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")

set(LU_SCAN_INCLUDE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/third_party/RakNet/Source")

set(LU_SCAN_LINK_LIBRARIES
    RakNet)

if(WIN32)
    list(APPEND LU_SCAN_LINK_LIBRARIES ws2_32)
endif(WIN32)

if(UNIX)
    list(APPEND LU_SCAN_LINK_LIBRARIES pthread)
endif(UNIX)

add_executable(lu_scan ${LU_SCAN_SOURCE_FILES})

target_include_directories(lu_scan PUBLIC ${LU_SCAN_INCLUDE_DIRECTORIES})

target_link_libraries(lu_scan PUBLIC ${LU_SCAN_LINK_LIBRARIES})