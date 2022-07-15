# Run via Docker

```
docker build --tag mathparser .
docker run --rm -p 8000:8000 mathparser
```

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
make install
```

For the UI you also need to have node, npm installed.

## Math Parser

This is how you build mathparser itself:

```bash
mkdir build && cd build
cmake ..
make
```

