build-gui-release: FORCE
	mkdir -p xcode
	cd xcode && cmake -GXcode -DCMAKE_BUILD_TYPE=Release ..
	cd xcode && xcodebuild
	cd FileSaver && xcodebuild

build-cli-release: FORCE
	mkdir build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/release && make -j

FORCE:
