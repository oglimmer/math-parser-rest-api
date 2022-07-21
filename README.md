# Run via Docker

Tested on ARMv8 only.

```
docker build --tag mathparser .
docker run --rm -p 8000:8000 mathparser
```

Then use it via `curl "http://localhost:8000/v1/calc?expression=3+4"`.

# Official hosting

The system is deployed at https://math.oglimmer.de

# Build

## Prerequisites

You need to have a c++11 compiler, cmake and conan installed. 

As this REST API uses my c++ math parser lib, you also need install this mathparser lib

```
git clone https://github.com/oglimmer/math_parser_cpp.git --depth=1
cd math_parser
mkdir build
cd build
conan install ..
cmake ..
cmake --build .
```

## Math Parser REST API

This is how you build REST API itself:

```bash
mkdir build && cd build
conan install .. --build=missing
cmake ..
cmake --build .
ctest
```

Now you can start it via:

```
build/bin/mathparser-exe
```

And you can use the local webserver:

```
curl "http://localhost:8000/v1/calc?expression=3+4"
```
