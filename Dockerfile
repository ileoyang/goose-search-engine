FROM ubuntu:20.04
ARG trecfilepath
ENV DEBIAN_FRONTEND noninteractive
WORKDIR /usr/src/app
RUN apt-get update \
    && apt-get install -y g++ \
    clang \
    cmake \
    && apt-get clean
COPY . .
RUN mkdir build-dir && cd build-dir && cmake .. && make
RUN mv $trecfilepath build-dir/msmarco-docs.trec
RUN cd build-dir && ./goose parse && ./goose merge && ./goose list