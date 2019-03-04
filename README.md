# Nova
![](https://i.imgur.com/CriXiIo.jpg?raw=true)

## Description
Nova is a 3d game engine(not yet usable) that i developed as a hobby trying to learn OpenGL and computer graphics in general, is very simple and is in a very rough state so keep that in mind.
Here are a few videos showing the engine https://www.youtube.com/channel/UCI-gu4Uz3p4gsd32sa1-YWA

## Getting Started
Nova has a bunch of dependencies, they are listed in a table in this document, most dependencies are already inside this repo if you are building on Windows and already have installed Visual Studio 2017 or later you are almost done, if you are on Linux you need your tool chain set up that means GCC, cmake, make and git.

### Intruction for all platforms
Download the resources(textures, 3d models etc) from:
https://drive.google.com/file/d/12aKuHqQie6UIXcSeQm_gk1i0riaNzQoI/view?usp=sharing

### Windows instructions:
1. Download the repo as a zip file by clicking the "clone or download" button.
2. Uncompress the zip file into a directory of your choice.
3. Create a new directory named bin inside the directory where you put your source files
3. Uncompress the resource directory resource.zip to the binary directory you just created
4. Copy the dir engine/resources to bin/engine/
5. Right click the source directory and select open with Visual Studio.
Visual Studio should be able to configure, compile and build the project for you.

### Linux(Ubuntu) instructions:
Open a terminal and type:
```bash
sudo apt install build-essential cmake git libpulse-dev portaudio19-dev libsndio-dev
git clone https://github.com/dakennedyd/Nova
cd Nova-development
mkdir bin
cd bin
cp -r ../engine/resources ./engine
```
Uncompress the resource directory resource.zip to the binary directory you just created
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j2
```

## Dependencies
You don't need to download this should be included inside this repo, this is just in case for some reason it is not

| Functionality  | Library |
| --- | --- |
| Mesh Loading | [assimp](https://github.com/assimp/assimp)   |
| Physics | [bullet](https://github.com/bulletphysics/bullet3)  |
| OpenGL Function Loader | [glad](https://github.com/Dav1dde/glad)      |
| Windowing and Input | [glfw](https://github.com/glfw/glfw)        |
| Texture and sound decoding | [stb](https://github.com/nothings/stb)   |
| Graphical UI | [Imgui](https://github.com/ocornut/imgui)  |
| Sound | [OpenAL Soft](https://github.com/kcat/openal-soft) |
| XML parser | [Tinyxml2](https://github.com/leethomason/tinyxml2)   |

## License
>The MIT License (MIT)

>Copyright (c) 2018 David Kennedy

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.