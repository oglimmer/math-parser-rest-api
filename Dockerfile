FROM debian:11-slim

RUN apt update && \
    apt install -y cmake g++ git doctest-dev

RUN cd /home &&  \
    git clone https://github.com/oatpp/oatpp.git --depth=1 &&  \
    cd oatpp/ && \
    mkdir build &&  \
    cd build &&  \
    cmake .. &&  \
    make install && \
    cd /home &&  \
    git clone https://github.com/oglimmer/oatpp-swagger.git --depth=1 && \
    cd oatpp-swagger/ &&  \
    mkdir build &&  \
    cd build &&  \
    cmake .. &&  \
    make install

RUN cd /home && \
    git clone https://github.com/oglimmer/math_parser.git --depth=1 && \
    cd math_parser && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

COPY . /home/mathparser

RUN cd /home/mathparser &&  \
    mkdir -p build &&  \
    cd build &&  \
    rm -rf * &&  \
    cmake .. &&  \
    make

FROM debian:11-slim

COPY --from=0 /usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res /usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res
COPY --from=0 /home/mathparser/build/mathparser-exe /usr/bin

CMD mathparser-exe
