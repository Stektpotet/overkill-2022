<<<<<<< HEAD
# TDT4230 - Graphics and Visualization

This is the base code for the NTNU subject TDT4230 assignments. It may also be used for the final project, which we will showcase [here](https://www.idi.ntnu.no/grupper/vis/teaching/)

## What do i do?

	git clone --recursive https://github.com/bartvbl/TDT4230-Assignment-1.git
=======
# TDT4230 - Graphics and Visualization Final Project


## What do i do?

	git clone --recursive https://github.com/Stektpotet/TDT4230-project.git
>>>>>>> 5ea8f4e9b1a543b8fce549c5d8a8017fa931b89f

Should you forget the `--recursive` bit, just run:

	git submodule update --init


### Windows

Install Microsoft Visual Studio Express and CMake.
You may use CMake-gui or the command-line cmake to generate a Visual Studio solution.

### Linux:

Make sure you have a C/C++ compiler such as  GCC, CMake and Git.

	make run

which is equivalent to

	git submodule update --init
	cd build
	cmake ..
	make
	./glowbox
