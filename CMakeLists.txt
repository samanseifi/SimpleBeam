cmake_minimum_required(VERSION 3.1)
project(SimpleBeam)

set(BINARY_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find BLAS and LAPACK libraries
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(includes/utility)

add_executable(${BINARY_NAME} src/main.cpp)

# Link BLAS and LAPACK libraries
# target_link_libraries(${BINARY_NAME} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})

target_link_libraries(${BINARY_NAME} ${LAPACK_LIBRARIES} lapacke)


# GoogleTest setup remains the same...


# Include GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows
FetchContent_MakeAvailable(googletest)

include(CTest)
enable_testing()

# Only include test subdirectory if BUILD_TESTING is on
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()