debug-build-and-test: dependencies FORCE
	@echo "Running DEBUG build and TEST"
	mkdir -p build/debug
	cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd build/debug && make -j
	./build/debug/bin/filesaver_tests

build-gui-release: dependencies FORCE
	mkdir -p xcode
	cd xcode && cmake -GXcode -DLEVELDB_BUILD_TESTS=OFF -DLEVELDB_INSTALL=OFF -DLEVELDB_BUILD_BENCHMARKS=OFF -DCMAKE_BUILD_TYPE=Release ..
	cd xcode && xcodebuild -configuration release
	cd FileSaver && xcodebuild -configuration release

build-cli-release: dependencies FORCE
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j

dependencies: FORCE
	mkdir -p dependencies
	cd dependencies && conan install --build missing ..

clean: FORCE
	rm -rf dependencies
	rm -rf build
	rm -rf xcode

FORCE:
