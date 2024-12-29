# Base ubuntu image
FROM ubuntu:latest

# Set to the location of the boost library
ENV BOOST_ROOT=/usr/include

# Install required packages
RUN apt-get update && apt-get install -y \
     cmake \
     g++ \
     libboost-program-options-dev \
     libboost-system-dev \
     libboost-filesystem-dev \
     libboost-serialization-dev \
     libboost-mpi-dev \
     doxygen && \
        rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Build the source code
RUN mkdir build && cd build && cmake ../src && make install
