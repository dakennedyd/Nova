#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ISingleton.h"
#include "ISubSystem.h"

namespace Nova {

	struct TextureInfo{
		//void * textureBuffer;
		//unsigned char * textureBuffer;
		//float * textureBufferFP;
		std::vector<void *> textureBuffer;
		int width;
		int height;
		int bpp;
		bool isHDR;
	};
	
	class FileSystem :public ISingleton<FileSystem>, public ISubSystem
	{
	public:
		FileSystem() = default;
		~FileSystem() = default;
		void startUp() override;
		void shutDown() override;

		std::string loadFileAsString(const std::string &pathAndFileName);

		/* don't call this function directly if you do you need
		to free the texture afterwards width freeTexture() */
		const TextureInfo loadTexture(const std::string& fileAndPath, const bool uvFlipped);		
		const TextureInfo loadTexture(const std::vector<std::string> filesAndPath, const bool uvFlipped);
		void freeTexture(void * ti);		

		const aiScene* loadModel(const std::string& pathAndFileName);

		std::vector<std::string> getFilenamesInDirectory(const std::string &path, bool recursive = true);
		std::string getFilenameExtension(const std::string &filenameAndPath);		
	private:		
	};
}
