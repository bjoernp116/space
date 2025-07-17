set working-directory := "./build"

_default:
    just --choose

clean:
    rm ./* -rf

build: 
    cmake ..
    cmake --build .

run: build
    ./Space

