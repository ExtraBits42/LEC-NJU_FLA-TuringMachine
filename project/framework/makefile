turing:
	sudo mkdir build
	cd ./build && sudo cmake .. && sudo make
	sudo cp ./bin/turing turing
	sudo rm -rf ./build ./bin

run: turing
	./turing

clean:
	sudo rm -rf ./build ./bin ./turing

test-none:
	./turing ./programs/demo.tm 1001001

test-v:
	./turing -v ./programs/demo.tm 1001001
