# Hello, 3D World!
This is my C++ project aimed at learning OpenGL. I use [LearnOpenGL](https://learnopengl.com/) as my resource.

### About libraries used
This project is made on a x86-64 linux machine, thus the provided GLFW library is compiled for this platform.
You can download GLFW's source code to compile for your own platform [here](https://www.glfw.org/).
[Glad](https://glad.dav1d.de/) and [stb](https://github.com/nothings/stb) (precisely stb_image.h) are also being used.

### Dependencies
- libglfw3 (included in the lib directory, compiled for x86-64 linux)
- libGL
- libX11
- libpthread
- libXrandr
- libXi
- libdl

### Compiling
G++:
```bash
g++ -I include -L lib -g main.cpp glad.c -o main.out -l glfw3 -l GL -l X11 -l pthread -l Xrandr -l Xi -l dl
```

### Other information
At the moment the program renders a textured rectangle.
Switching between texture zoom modes is done by clicking '1' or '2' keys.
Increasing or reducing the texture mix value is done by holding the up arrow or the down arrow key.
Hitting the '0' key exits the program.
