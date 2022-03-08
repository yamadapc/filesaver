debug-build-and-test: FORCE
	@echo "Running DEBUG build and TEST"
	make debug-build
	./build/debug/bin/filesaver_tests

debug-build: FORCE
	mkdir -p build/debug
	cd build/debug && cmake -DENABLE_CPPCHECK=ON -DENABLE_CLANG_TIDY=ON ../..
	cd build/debug && make -j

release-build-and-test: FORCE
	@echo "Running RELEASE build and TEST"
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j
	./build/release/bin/filesaver_tests

gui-conan: FORCE
	mkdir -p FileSaver/conan
	cd FileSaver/conan && conan install .. --build --profile ../conan.prof

build-gui-release: FORCE
	make xcode
	make gui-conan
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
	cd build/release && cmake -DCMAKE_OSX_ARCHITECTURES=arm64 -DCONAN_ARCH=armv8 -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j

build-cli-release-x86: FORCE
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCONAN_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j

xcode: FORCE
	mkdir -p xcode
	cd xcode && cmake -GXcode -DWARNINGS_AS_ERRORS=OFF -DCMAKE_BUILD_TYPE=Release ..
	cd xcode && xcodebuild -configuration release

clean: FORCE
	rm -rf build
	rm -rf xcode

FORCE:
