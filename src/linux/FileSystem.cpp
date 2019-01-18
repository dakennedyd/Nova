
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include "linux/FileSystem.h"
#include "Error.h"
#include "logger/Logger.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>
#include <vector>
#include"Init.h"
#ifdef NOVA_OPENGL
    #include "graphics/opengl/RendererInit.h"
#endif

namespace Nova
{
void FileSystem::startUp()
{
    // stbi_set_flip_vertically_on_load(true);
    initDirWatcher();
}
void FileSystem::shutDown() 
{ 
    deInitDirWatcher();
}
std::string FileSystem::loadFileAsString(const std::string &pathAndFileName)
{
    std::ifstream ifs(pathAndFileName.c_str(), std::ios::in /*| std::ios::binary*/ | std::ios::ate);

    if (!ifs.is_open())
    {
        error("Can't open file:" + pathAndFileName);
    }
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(&bytes[0], fileSize);

    return std::string(&bytes[0], fileSize);
}
const TextureInfo FileSystem::loadTexture(const std::string &fileAndPath, const bool uvFlipped)
{
    stbi_set_flip_vertically_on_load(uvFlipped);
    bool isHDR = stbi_is_hdr(fileAndPath.c_str());
    int width, height, bytesPerPixel;
    LOG_DEBUG("Loading texture:" << fileAndPath);
    unsigned char *buffer = nullptr;
    float *bufferFP = nullptr;
    if (isHDR)
    {
        bufferFP = stbi_loadf(fileAndPath.c_str(), &width, &height, &bytesPerPixel, 0);
        if (!bufferFP)
        {
            error("could not open file:" + fileAndPath);
        }
    }
    else
    {
        buffer = stbi_load(fileAndPath.c_str(), &width, &height, &bytesPerPixel, 0);
        if (!buffer)
        {
            error("could not open file:" + fileAndPath);
        }
    }
    // LOG_INFO("Load successful. width:" << width << " height:" << height << " bpp:" <<
    // bytesPerPixel * 8);
    TextureInfo t;
    // t.textureBuffer = buffer;
    // t.textureBufferFP = bufferFP;
    if (isHDR)
        t.textureBuffer.push_back(bufferFP);
    else
        t.textureBuffer.push_back(buffer);
    t.width = width;
    t.height = height;
    t.bpp = bytesPerPixel;
    t.isHDR = isHDR;
    return t;
}
const TextureInfo FileSystem::loadTexture(const std::vector<std::string> filesAndPath,
                                          const bool uvFlipped)
{
    stbi_set_flip_vertically_on_load(uvFlipped);
    TextureInfo t;
    int width, height, bytesPerPixel;
    bool isHDR;
    int c = 0;
    for (auto &file : filesAndPath)
    {
        isHDR = stbi_is_hdr(file.c_str());
        LOG_DEBUG("Loading texture:" << file);
        unsigned char *buffer = nullptr;
        float *bufferFP = nullptr;
        if (isHDR)
        {
            bufferFP = stbi_loadf(file.c_str(), &width, &height, &bytesPerPixel, 0);
            if (!bufferFP)
            {
                error("could not open file:" + file);
            }
        }
        else
        {
            buffer = stbi_load(file.c_str(), &width, &height, &bytesPerPixel, 0);
            if (!buffer)
            {
                error("could not open file:" + file);
            }
        }
        if (isHDR)
            t.textureBuffer.push_back(bufferFP);
        else
            t.textureBuffer.push_back(buffer);
        if (c == 0)
        {
            t.width = width;
            t.height = height;
            t.bpp = bytesPerPixel;
            t.isHDR = isHDR;
        }
        c++;
    }
    // LOG_INFO("Load successful. width:" << width << " height:" << height << " bpp:" <<
    // bytesPerPixel * 8); t.textureBuffer = buffer; t.textureBufferFP = bufferFP;
    return t;
}
void FileSystem::freeTexture(void *ti) { stbi_image_free(ti); }

const aiScene *FileSystem::loadModel(const std::string &pathAndFileName)
{
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(pathAndFileName, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        error("ERROR::ASSIMP::" + std::string{import.GetErrorString()});
    }

    return scene;
}

std::vector<std::string> FileSystem::checkIfShadersChanged()
{
    std::vector<std::string> result;
    if (poll(&mFileDescriptorData, 1, 0) > 0)
    {
        /*some non portable alignment shiz(gcc only i dont know about clang etc),
        you can live without it on x86 is probably gonna be a little bit slower
        but in this case speed is of no concern*/
        char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
        const struct inotify_event *event;
        ssize_t readOffset;
        char *ptr;

        /* Read some events. */
        readOffset = read(mFileDescriptor, buffer, sizeof buffer);
        if (readOffset == -1) // && errno != EAGAIN)
        {
            LOG_ERROR("Inotify:Can't read events");
        }

        /* Loop over all events in the buffer */
        for (ptr = buffer; ptr < buffer + readOffset;
             ptr += sizeof(struct inotify_event) + event->len)
        {
            //event = (const struct inotify_event *)ptr;
            event = (const struct inotify_event *)buffer;
            if (event->len)
            {
                //std::cout << event->name << "\n";                
                std::string shaderFile = event->name;
                if(shaderFile.substr(shaderFile.size()-5, shaderFile.size()) == ".glsl")
                {
                    shaderFile = "shaders/" + shaderFile;                
                    shaderFile = shaderFile.substr(0, shaderFile.size()-5);
                    result.push_back(shaderFile);
                }
            }
        }
    }
    return result; 
}

void FileSystem::initDirWatcher()
{
    /* Create the file descriptor for accessing the inotify API */
    mFileDescriptor = inotify_init1(IN_NONBLOCK);
    if (mFileDescriptor == -1)
    {
        LOG_ERROR("Inotify:Can't initialize the API");
    }

    /* Mark directories for events */
    std::string s = PATH_TO_ENGINE_BINARY + SHADERS_PATH;
    s = s.substr(0,s.size()-1);
    const char *dirPath = s.c_str();
    //const char *dirPath = "/home/david/src/project_nova/TestGame/bin/engine/Resources/shaders/glsl";
    mWatchDescriptor =
        inotify_add_watch(mFileDescriptor, dirPath, /*IN_OPEN | IN_CLOSE*/ IN_MODIFY);
    if (mWatchDescriptor == -1)
    {
        LOG_ERROR("Inotify:Can't create dir watcher")
    }

    /* Prepare for polling */
    mFileDescriptorData.fd = mFileDescriptor;
    mFileDescriptorData.events = POLLIN;
}

void FileSystem::deInitDirWatcher()
{
    /* Close inotify file descriptor */
    close(mFileDescriptor);
}
// std::vector<std::string> FileSystem::getFilenamesInDirectory(const std::string & path, bool
// recursive)
// {
// 	#ifdef NOVA_WINDOWS_PLATFORM
// 	HANDLE dir;
// 	WIN32_FIND_DATA file_data;
// 	std::vector<std::string> out;
// 	std::string mypath(path);
// 	if (mypath[mypath.length()-1] != '/' && mypath[mypath.length()-1] != '\\')
// mypath.push_back('/'); 	if ((dir = FindFirstFile((mypath + "*").c_str(), &file_data)) ==
// INVALID_HANDLE_VALUE) 		return std::vector<std::string>(); /* No files found */

// 	do {
// 		const std::string file_name = file_data.cFileName;
// 		const std::string full_file_name = mypath + file_name;
// 		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) !=
// 0;

// 		if (file_name[0] == '.') continue;

// 		if (is_directory)
// 		{
// 			if (recursive)
// 			{
// 				std::vector<std::string> temp =
// getFilenamesInDirectory(full_file_name,
// true); 				out.insert(out.end(), temp.begin(), temp.end());
// 			}
// 			continue;
// 		}

// 		out.push_back(full_file_name);
// 		//LOG_DEBUG(full_file_name);
// 	} while (FindNextFile(dir, &file_data));

// 	FindClose(dir);
// 	return out;
// 	#else
// 			// DIR *dir;
// 			// class dirent *ent;
// 			// class stat st;

// 			// dir = opendir(directory);
// 			// while ((ent = readdir(dir)) != NULL) {
// 			// 	const string file_name = ent->d_name;
// 			// 	const string full_file_name = directory + "/" + file_name;

// 			// 	if (file_name[0] == '.')
// 			// 		continue;

// 			// 	if (stat(full_file_name.c_str(), &st) == -1)
// 			// 		continue;

// 			// 	const bool is_directory = (st.st_mode & S_IFDIR) != 0;

// 			// 	if (is_directory)
// 			// 		continue;

// 			// 	out.push_back(full_file_name);
// 			// }
// 			// closedir(dir);
// 	#endif
// 	return std::vector<std::string>();
// }
// std::string FileSystem::getFilenameExtension(const std::string & filenameAndPath)
// {
// 	// /*note: this probably would not work on unicode*/

// 	// //return PathFindExtension(filenameAndPath.c_str());
// 	// std::string extension(PathFindExtension(filenameAndPath.c_str()));
// 	// /*PathFindExtension does not work with files with no extension*/
// 	// //std::replace(extension.begin(), extension.end(), '\\', '/');
// 	// replaceAllInPlace(extension, "\\", "/");
// 	// if (extension.find("/") == std::string::npos)
// 	// {
// 	// 	extension = extension.substr(1, extension.size()); //remove the dot character
// 	// 	replaceAllInPlace(extension, "//", "/");
// 	// 	return extension;
// 	// }
// 	// else {
// 	// 	//extension = extension.substr(extension.find_last_of("/") + 1);
// 	// 	return std::string();
// 	// }
// 	// //get file name from file path
// 	// //std::string filename(filenameAndPath.substr(filenameAndPath.find_last_of("/") + 1));

// 	// //get extension
// 	// //std::string extention(filename.substr(filename.find_last_of(".") + 1));
// 	return std::string{"-"};
// }
} // namespace Nova