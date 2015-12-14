# cpp-challenge setup

### Ubuntu ###
This is tested only on Ubuntu 14.04.3 LTS

1. Download and extract the latest CMake release (as of this guide it's on 3.4.1)(https://cmake.org/download/) 
	1. Open the terminal and go to the folder where you extracted.
	2. Use this commands to install:
	
          ./bootstrap

          make

          sudo make install

2. Download and extract the latest ZeroMQ release (as of this guide it's on 4.1.3)(http://zeromq.org/intro:get-the-software)
	1. Open the terminal and go to the folder where you extracted.
	2. Make sure that libtool, pkg-config, build-essential, autoconf, and automake are installed.
	3. Check whether uuid-dev package, uuid/e2fsprogs RPM or equivalent on your system is installed.
	4. Use this commands to install:
		
        ./configure --without-libsodium
		
        make
		
        sudo make install
		
        sudo ldconfig


3. Download and extract the latest OpenCV release (as of this guide it's on 3.0)(http://opencv.org/)
	1. Open the terminal and go to the folder where you extracted.
	2. Make sure that you run this commands.
	
		sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade && sudo apt-get autoremove
	4. Install the Media I/O: sudo apt-get install zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev
	5. Use this commands to install: 
		
        mkdir build
		
        cd build
		
        cmake -DWITH_QT=ON -DWITH_OPENGL=OFF -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON -DBUILD_EXAMPLES=ON ..
		
        make -j4
		
        sudo make install
		
        sudo ldconfig

4. Download the files for the server application from https://github.com/minos55/cpp-challenge 
	1. Open the terminal and go to where you downloaded.
	2. Use this command to build the program:
		mkdir build && cd build && cmake .. && make
	3. Use this command to run the program:
		
        ./cpp_challenge




