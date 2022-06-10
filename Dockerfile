FROM ubuntu:20.04
ARG trecfilepath
ENV DEBIAN_FRONTEND noninteractive
WORKDIR /usr/src/app
# Install engine packages.
RUN apt-get update && apt-get install -y g++ clang cmake && apt-get clean
COPY . .
# Compile engine programs.
RUN mkdir build-dir build-dir/query-res && cd build-dir && cmake .. && make
# Add the document dataset.
RUN mv $trecfilepath build-dir/msmarco-docs.trec
# Load the engine runtime environment.
RUN cd build-dir && ./goose parse && ./goose merge && ./goose list
# Install Node.js 18.
RUN apt-get install -y curl && curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && apt-get install -y nodejs
# Install API packages.
RUN cd api && npm i
# Install frontend packages and create a production build.
RUN cd frontend && npm i && npm run build
EXPOSE 5000
CMD cd api && node index.js