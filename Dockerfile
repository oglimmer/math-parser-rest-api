FROM debian:11-slim

RUN apt update && \
    apt install -y cmake git python3-pip && \
    pip3 install conan==1.59

RUN cd /home && \
    git clone https://github.com/oglimmer/math_parser_cpp.git --depth=1 && \
    cd math_parser_cpp && \
    mkdir build && \
    cd build && \
    conan install .. && \
    cmake .. &&  \
    cmake --build . &&  \
    ctest && \
    cmake --install .

COPY . /home/mathparser

RUN cd /home/mathparser &&  \
    mkdir -p build &&  \
    cd build &&  \
    rm -rf * && \
    conan install .. --build=oatpp --build=oatpp-swagger --profile ../conan-profile.armv8 && \
    cp -r $(awk -F "=" '/OATPP_SWAGGER_RES_PATH/ {print $2}' ./conanbuildinfo.txt) /usr/local/oatpp-swagger-res && \
    cmake .. && \
    cmake --build .

FROM debian:11-slim

COPY --from=0 /usr/local/oatpp-swagger-res /usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res
COPY --from=0 /home/mathparser/build/bin/mathparser-exe /usr/bin

CMD mathparser-exe
