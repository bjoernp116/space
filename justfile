
_default:
    just --choose

[working-directory: './build']
clean:
    rm ./* -rf

[working-directory: './src']
fmt: 
    clang-format -i ./*.h ./*.cpp

[working-directory: './build']
build: fmt 
    cmake ..
    cmake --build .

run: build
    ./build/Space

debug: build
    gdb ./build/Space
