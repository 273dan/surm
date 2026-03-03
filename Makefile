.PHONY: all build run config

all: run

config:
	cmake -S . -B build
build:	config
	cmake --build build
run:	build
	./build/surm
install:	build
	sudo cp ./build/surm /usr/bin/surm
	sudo chmod +x /usr/bin/surm


