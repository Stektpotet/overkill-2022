


https://user-images.githubusercontent.com/2846061/167323010-ac4a7231-8620-4015-94ab-38c7914028d5.mp4



# TDT4230 - Graphics and Visualization Final Project


## What do I do?

	git clone --recursive https://github.com/Stektpotet/TDT4230-project.git
Should you forget the `--recursive` bit, just run:

	git submodule update --init


### Windows

Install Microsoft Visual Studio Express and CMake.
You may use CMake-gui or the command-line cmake to generate a Visual Studio solution.

### Linux:

Untested; might not compile correctly

Make sure you have a C/C++ compiler such as  GCC, CMake and Git.

	make run

which is equivalent to

	git submodule update --init
	cd build
	cmake ..
	make
	./overkill
