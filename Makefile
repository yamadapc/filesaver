debug-build-and-test: FORCE
	@echo "Running DEBUG build and TEST"
	mkdir -p build/debug
	cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd build/debug && make -j
	./build/debug/bin/filesaver_tests

release-build-and-test: FORCE
	@echo "Running RELEASE build and TEST"
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j
	./build/release/bin/filesaver_tests

build-gui-release: FORCE
	make xcode
	mkdir -p FileSaver/conan
	cd FileSaver/conan && conan install .. --build --profile ../conan.prof
	cd FileSaver && xcodebuild -configuration release

# Requires github-release https://github.com/github-release/github-release
app-release: FORCE
	make build-gui-release
	mkdir -p ./releases/$(shell git describe --tags)
	mv ./FileSaver/build/Release/FileSaver.app ./releases/$(shell git describe --tags)/
	cd ./releases/$(shell git describe --tags)/ && zip -r FileSaver.zip ./FileSaver.app
	github-release upload \
		--user yamadapc \
		--repo filesaver \
		--tag $(shell git describe --tags) \
		--name "FileSaver.zip" \
		--file ./releases/$(shell git describe --tags)/FileSaver.zip

build-cli-release: FORCE
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j

xcode: FORCE
	mkdir -p xcode
	cd xcode && cmake -GXcode -DWARNINGS_AS_ERRORS=OFF -DCMAKE_BUILD_TYPE=Release ..
	cd xcode && xcodebuild -configuration release

clean: FORCE
	rm -rf build
	rm -rf xcode

FORCE:
