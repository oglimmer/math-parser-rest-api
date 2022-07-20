# Run via Docker

```
docker build --tag mathparser .
docker run --rm -p 8000:8000 mathparser
```

Then use it via `curl "http://localhost:8000/v1/calc?expression=3+4"`.

# Official hosting

The system is deployed at https://math.oglimmer.de

# Build

## Prerequisites

You need to have oat++ and oat-swagger installed, see https://oatpp.io/docs/installation/unix-linux/ 

For Unix/macOS do this:

```
# oatpp
git clone https://github.com/oatpp/oatpp.git --depth=1
cd oatpp/
mkdir build
cd build
cmake ..
make install

# oatpp-swagger
git clone https://github.com/oglimmer/oatpp-swagger.git --depth=1
cd oatpp-swagger/
mkdir build
cd build
cmake ..
make install
```

We also need the mathparser lib

```
git clone https://github.com/oglimmer/math_parser.git --depth=1
cd math_parser
mkdir build
cd build
cmake ..
make install
```

## Math Parser REST API

This is how you build REST API itself:

```bash
mkdir build && cd build
cmake ..
make
```

Now you can start it via:

```
build/mathparser-exe
```

And you can use the local webserver:

```
curl "http://localhost:8000/v1/calc?expression=3+4"
```
