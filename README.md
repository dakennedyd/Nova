# Nova
![](https://i.imgur.com/CriXiIo.jpg?raw=true)
[![Build Status](https://travis-ci.com/dakennedyd/Nova.svg?token=sNzFKmUxnz5KdnspDAiK&branch=development)](https://travis-ci.com/dakennedyd/Nova)

## Description
Nova is a 3d game engine(not yet usable) that i developed as a hobby trying to learn OpenGL and computer graphics in general, is very simple and is in a very rough state so keep that in mind.
Here are a few videos showing the engine https://www.youtube.com/channel/UCI-gu4Uz3p4gsd32sa1-YWA

### Linux(Ubuntu 16.04) build instructions:
Open a terminal and type:
```bash
sudo apt update -y
sudo apt install -y build-essential cmake git libxinerama-dev libxcursor-dev xorg-dev libpulse-dev portaudio19-dev libsndio-dev
git clone --recursive https://github.com/dakennedyd/Nova
cd Nova
mkdir bin
cd bin
```
Download the resources(textures, 3d models etc) from:
https://drive.google.com/open?id=1CWzjkL3k5ac-coCcCM6IyrDKfIiEifFI
Copy the "Resources" directory from Resources.zip to the directory "bin" you just created

then type
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j2
```

## Dependencies
You don't need to download this it should be included inside this repo, this is just for completeness

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

>Copyright (c) 2017-2019 David Kennedy

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.