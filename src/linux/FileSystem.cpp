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

namespace Nova
{
void FileSystem::startUp()
{
    // stbi_set_flip_vertically_on_load(true);
}
void FileSystem::shutDown() {}
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