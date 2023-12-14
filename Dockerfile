FROM ubuntu:22.04
LABEL authors="artemysazonov"

RUN apt-get update &&  \
    apt-get -y upgrade &&  \
    apt-get -y install g++ git doxygen cmake &&  \
    mkdir FTQuant

COPY . ./FTQuant

WORKDIR /FTQuant

RUN mkdir ./build && cd build && cmake ..
RUN cd tests && ctest


