FROM ubuntu:20.04

ARG trecfilepath
ENV DEBIAN_FRONTEND noninteractive
WORKDIR /usr/src/app
COPY . .

# Install engine packages.
RUN apt-get update && apt-get install -y g++ clang cmake && apt-get clean

# Install Restbed.
RUN apt-get install git -y
RUN git clone --recursive https://github.com/corvusoft/restbed.git
RUN mkdir restbed/build && cd restbed/build && cmake -DBUILD_SSL=NO -DBUILD_TESTS=NO .. && make install

# Install nlohmann/json.
RUN apt-get install nlohmann-json3-dev -y

# Compile engine programs.
RUN mkdir build-dir && cd build-dir && cmake .. && make

# Add the document dataset.
RUN mv $trecfilepath build-dir/msmarco-docs.trec

# Load the engine runtime environment.
RUN cd build-dir && ./goose parse && ./goose merge && ./goose list

EXPOSE 5000
CMD cd build-dir && ./goose -api
