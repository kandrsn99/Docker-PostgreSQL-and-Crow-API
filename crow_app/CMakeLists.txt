# Minimum Version for CMAKE
cmake_minimum_required(VERSION 3.10)
project(CrowApp) # Create the Project
# Crow makes usage of C++ 17 Standards.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# We will use the FetchContent to download our necessary repositories for the libraries we shall use.
include(FetchContent)
# We will use the FetchContent to download the Asio Repository. Thus, ensuring it is in our docker environment for Crow.
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git # https://github.com/boostorg/asio.git
  GIT_TAG master  # Replace with your favorite tag.
)
FetchContent_MakeAvailable(asio)
# We have to set the directory, otherwise crow won't have a file to find for building. We also set the flag to true so that it is read properly.
set(ASIO_INCLUDE_DIR "${asio_SOURCE_DIR}/asio/include" CACHE PATH "ASIO include directory")
set(ASIO_FOUND TRUE CACHE BOOL "ASIO is found")
include_directories(${ASIO_INCLUDE_DIR})
# Get the Crow repository.
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG master  # Specification for release or commit tag from the repository.
)
FetchContent_MakeAvailable(crow)
# Create the target library alias for crow when it is linked later for server.cpp file.
if(NOT TARGET crow::crow)
    add_library(crow INTERFACE)
    target_include_directories(crow INTERFACE ${crow_SOURCE_DIR}/include)
    add_library(crow::crow ALIAS crow)
endif()
# FetchContent to download the libpqxx library for us to use.
FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
  GIT_TAG master # Replace with your favorite tag.
)
FetchContent_MakeAvailable(libpqxx)
# If we recieved pqxx_static we create an alias for when pqxx is linked later for server.cpp file.
if(TARGET pqxx_static AND NOT TARGET pqxx::pqxx)
    add_library(pqxx::pqxx ALIAS pqxx_static)
endif()
# Otherwise, if we have pqxx we add the library with the proper alias.
if(TARGET pqxx AND NOT TARGET pqxx::pqxx)
    add_library(pqxx::pqxx ALIAS pqxx)
endif()
# Add the cpp file and subsequent executable.
add_executable(server server.cpp)
# Make sure to include the directories so that we may find crow.h (your header) in the server.cpp file.
# This is the solution to a problem with docker environments for finding the crow header.
target_include_directories(server PRIVATE ${crow_SOURCE_DIR}/include)
# Thread support for crow (multithreading).
find_package(Threads REQUIRED)
# Target libraries during the build
target_link_libraries(server PRIVATE crow::crow pqxx::pqxx Threads::Threads)
