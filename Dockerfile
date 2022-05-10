FROM ubuntu:20.04
ARG trecfilepath
ENV DEBIAN_FRONTEND noninteractive
WORKDIR /usr/src/app
RUN apt-get update && apt-get install -y g++ clang cmake && apt-get clean
COPY . .
RUN mkdir build-dir build-dir/query-res && cd build-dir && cmake .. && make
RUN mv $trecfilepath build-dir/msmarco-docs.trec
RUN cd build-dir && ./goose parse && ./goose merge && ./goose list
RUN apt-get install -y curl && curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && apt-get install -y nodejs
RUN cd api && npm i
EXPOSE 3000
CMD cd api && node index.js