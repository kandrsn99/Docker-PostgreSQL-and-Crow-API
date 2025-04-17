# Minimum Version for CMAKE
cmake_minimum_required(VERSION 3.10)
project(CrowApp) # Create the Project
# Crow makes usage of C++ 17 Standards.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# We will use the FetchContent to download the Crow Repository. Thus, ensuring it is in our docker environment.
include(FetchContent)
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG master  # Specification for release or commit tag from the repository.
)
FetchContent_MakeAvailable(crow)
# Add the cpp file and subsequent executable.
add_executable(server server.cpp)
# Make sure to include the directories so that we may find crow.h (your header). This a problem with docker environments for finding the crow header.
target_include_directories(server PRIVATE ${crow_SOURCE_DIR}/include)
# Thread support for crow.
find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)
