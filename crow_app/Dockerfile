# Latest GCC image
FROM gcc:latest
# Run installation of necessary packages for the CMake and crow header.
RUN apt update && apt install -y \
    g++ \
    cmake \
    make \
    git
# Create a working directory and copy everything over in our current file path.
WORKDIR /app
COPY . .
# Build the file with our crow header.
RUN cmake -S . -B build && cmake --build build
# Expose the proper port. Please note that we supersede the firewall and open this port. 
EXPOSE 8080
# Run the API written in server.cpp
CMD ["./build/server"]
