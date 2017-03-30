# Polygon-Demo
OpenGL Tests - Linux - Polygon Demo


### build demo

```sh
# Download Demo
git clone https://github.com/blackmutzi/Polygon-Demo/
cd ./Polygon-Demo

# Makefile Generation
./autogen.sh

# Build Demo 
cd ./build
make
```

### prepare & start demo 

```sh
# Extract resource.tar.gz 
# included shader codes, images and mesh resources ...
# go to build directory 
cd ./build 
# extract *tar.gz
tar -zxvf resource.tar.gz
# start demo 
./polygon-demo 
```

### demo control

* W,A,S,D - control the Camera 
* R - Q   - Enable/Disable Water surface 


### demo dependencies

* Polygon-Engine
* Polygon-Component 

### other library dependencies

* LZ4
* GLFW
* OpenGL (Mesa)
* FreeType
* FreeImage
* Boost
