.PHONY: all build run config

all: build

config:
	cmake -S . -B build
build:	config
	cmake --build build
run:	build
	./build/surm
install:	build
	sudo cp ./build/surm /usr/bin/surm
	sudo chmod +x /usr/bin/surm
test:	build
	python3 ./tests/test_runner.py
test-v:	build
	python3 ./tests/test_runner.py -v


