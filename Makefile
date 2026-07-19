debug:
	cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build/debug

dev:
	cmake -B build/dev -DCMAKE_BUILD_TYPE=Dev
	cmake --build build/dev

release:
	cmake -B build/release -DCMAKE_BUILD_TYPE=Release
	cmake --build build/release

clean:
	rm -rf build bin