debug-build-and-test: FORCE
	@echo "Running DEBUG build and TEST"
	make debug-build
	./build/debug/bin/filesaver_tests

debug-build: FORCE
	mkdir -p build/debug
	# cd build/debug && cmake -DENABLE_CPPCHECK=ON -DENABLE_CLANG_TIDY=ON ../..
	cd build/debug && cmake ../..
	cd build/debug && make -j

release-build-and-test: FORCE
	@echo "Running RELEASE build and TEST"
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j
	./build/release/bin/filesaver_tests

FileSaver/fat_conan.xcconfig: FORCE
	mkdir -p FileSaver/conan
	cd FileSaver/conan && conan install .. --build --profile ../conan.prof
	mkdir -p FileSaver/conan-x86
	cd FileSaver/conan-x86 && conan install .. --build --profile ../conan_x86.prof
	make fix-xcconfig

fix-xcconfig: FORCE
	cat ./FileSaver/conan/conanbuildinfo.xcconfig | sed 's/ =/[arch=arm64] =/' > ./FileSaver/fat_conan.xcconfig
	cat ./FileSaver/conan-x86/conanbuildinfo.xcconfig | sed 's/ =/[arch=x86_64] =/' >> ./FileSaver/fat_conan.xcconfig

build-gui-release: FORCE
	make FileSaver/fat_conan.xcconfig
	cd FileSaver && xcodebuild -project ./Parallel\ Disk\ Scanner.xcodeproj -scheme FileSaver -configuration Release

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
	mkdir -p build/release_x86
	cd build/release_x86 && cmake -DCMAKE_OSX_ARCHITECTURES=x86_64 -DCONAN_ARCH=x86_64 -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release_x86 && make -j

build-fat-release: build-cli-release build-cli-release-x86
	mkdir -p build/release_fat
	lipo -info ./build/release/lib/liblfilesaver.a
	lipo -info ./build/release_x86/lib/liblfilesaver.a

	mkdir -p build/release_fat/lib
	mkdir -p build/release_fat/bin
	lipo -create ./build/release_x86/lib/liblfilesaver.a ./build/release/lib/liblfilesaver.a -output ./build/release_fat/lib/liblfilesaver.a
	lipo -create ./build/release_x86/bin/filesaver ./build/release/bin/filesaver -output ./build/release_fat/bin/filesaver

xcode: FORCE
	mkdir -p xcode
	cd xcode && cmake -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -DCONAN_ARCH="armv8;x86_64" -GXcode -DWARNINGS_AS_ERRORS=OFF -DCMAKE_BUILD_TYPE=Release ..
	cd xcode && xcodebuild -configuration release

xcode-x86: FORCE
	mkdir -p xcode-x86
	cd xcode-x86 && cmake -DCONAN_ARCH=x86_64 -DCMAKE_OSX_ARCHITECTURES=x86_64 -GXcode -DWARNINGS_AS_ERRORS=OFF -DCMAKE_BUILD_TYPE=Release ..
	cd xcode-x86 && xcodebuild -configuration release
	mv xcode-x86/filesaver.xcodeproj xcode-x86/filesaver_x86.xcodeproj

clean: FORCE
	rm -rf build
	rm -rf xcode

FORCE:
