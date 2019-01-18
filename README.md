# Nova
![Screenshot](https://imgur.com/a/bwfs7sG)

## Description
Nova is a 3d game engine that i developed as a hobby trying to learn OpenGL and computer graphics in general, is very simple and is in a very rough state so keep that in mind.

## Getting Started
Nova has a bunch of dependencies, they are listed in a table in this document, most dependencies are already inside this repo if you are building on Windows and already have installed Visual Studio 2017 you are all set, if you are on Linux you need your tool chain set up that means GCC, cmake, make and git.

### Windows instructions:
1. Download the repo as a zip file clicking the cloud button.
2. Uncompress the zip file into a directory of your choice.
3. Right click that directory and select open with Visual Studio.
Visual Studio should be able to configure, compile and build the project for you.

### Linux(Ubuntu) instructions:
Open a terminal and type:
```bash
sudo apt install build-essential cmake git libpulse-dev portaudio19-dev libsndio-dev
git clone https://gitlab.com/dakennedy/Nova
cd Nova
mkdir bin
cd bin
cmake ..
make
cp -r ..Resources .
```

## Dependencies
You don't need to download this is all included inside this repo, this is just for reference
|Functionality           	| Library                                                           |
|----------------------- 	|------------------------------------------                         |
|Mesh Loading            	| [assimp](https://github.com/assimp/assimp)                        |
|Physics                 	| [bullet](https://github.com/bulletphysics/bullet3)                |
|OpenGL Function Loader  	| [glad](https://github.com/Dav1dde/glad)                           |
|Windowing and Input     	| [glfw](https://github.com/glfw/glfw)                              |
|Texture and sound decoding	| [stb](https://github.com/nothings/stb)                            |
|Graphical UI				| [Imgui](https://github.com/ocornut/imgui)                         |
|Sound						| [OpenAL Soft](https://github.com/kcat/openal-soft) (LGPL licensed)|
|XML parser					| [Tinyxml2](https://github.com/leethomason/tinyxml2)               |

## License
>The MIT License (MIT)

>Copyright (c) 2018 David Kennedy

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.