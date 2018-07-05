#include <utility>
#include <vector>
#include <cmath>
#include <string>
#include <GLFW/glfw3.h>
#include "TextureCube.h"
#include "Error.h"
#ifdef NOVA_WINDOWS_PLATFORM
	#include "windows/FileSystem.h"
#else
	#include "linux/FileSystem.h"
#endif

namespace Nova {	
	TextureCube::~TextureCube()
	{
		glDeleteTextures(1, &mTextureID);
	}

	TextureCube::TextureCube(TextureCube && other)
		:mTextureID(std::move(other.mTextureID)), mWidth(std::move(other.mWidth)),
		mHeight(std::move(other.mHeight)), mHasMipmaps(std::move(other.mHasMipmaps)),
		mType(std::move(other.mType)), mTextureBuffer(std::move(other.mTextureBuffer))
	{		
		mTextureID = 0;
		mTextureBuffer = nullptr;
	}

	TextureCube & TextureCube::operator=(TextureCube && other)
	{
		if (&other != this) {
			mTextureID = std::move(other.mTextureID);
			mWidth = std::move(other.mWidth);
			mHeight = std::move(other.mHeight);
			mHasMipmaps = std::move(other.mHasMipmaps);
			mType = std::move(other.mType);
			mTextureBuffer = std::move(other.mTextureBuffer);
			//mTextureBuffer = other.mTextureBuffer;			
		}
		return *this;
	}

	//TextureCube::TextureCube(const std::array<TextureInfo, 6>& textureInfoArray)
	//{
	//	glGenTextures(1, &mTextureID);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	//	// Set the texture wrapping option
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//	// Set the texture filtering options
	//	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		

	//	//glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);	//fix: check if supported!!

	//	//uploads texture data to GPU	
	//	GLuint texFormat = 0;
	//	for(int i=0; i < 6; i++)
	//	{			
	//		if (textureInfoArray[i].bpp == 3) texFormat = GL_RGB;			
	//		else if (textureInfoArray[i].bpp == 4) texFormat = GL_RGBA;
	//		else
	//		{
	//			error("unrecognized cube texture buffer");				
	//		}
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	//			0,
	//			texFormat,
	//			textureInfoArray[i].width,
	//			textureInfoArray[i].height,
	//			0,
	//			texFormat,
	//			GL_UNSIGNED_BYTE,
	//			textureInfoArray[i].textureBuffer);

	//		FileSystem::getInstance().freeTexture(textureInfoArray[i].textureBuffer); //just temp until the resource mgr is implemented
	//	}
	//				
	//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//}

	TextureCube::TextureCube(const std::vector<void*>textureData, int width, int height, int bpp, bool isHDR)
	{
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

		// Set the texture wrapping option
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Set the texture filtering options
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 4);
		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		if (textureData.size() > 1) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
		//uploads texture data to GPU	
		GLuint texFormat = 0;
		if (isHDR) {
			int mipLevel = 0;
			for (auto data : textureData) {
				double texWidth = width * std::pow(0.5, mipLevel), texHeight = height* std::pow(0.5, mipLevel);
				for (int i = 0; i < 6; i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						mipLevel,
						GL_RGB32F, //maybe 32f?
						static_cast<GLsizei>(texWidth),
						static_cast<GLsizei>(texHeight / 6),
						0,
						GL_RGB,
						GL_FLOAT,
						(const float *)textureData[mipLevel] + static_cast<int>(texWidth*(texHeight / 6)*bpp*i));
				}
				mipLevel++;
			}
		}else{
			if (bpp == 3) texFormat = GL_RGB;
			else if (bpp == 4) texFormat = GL_RGBA;
			else
			{
				error("unrecognized texture buffer");
			}
			//for (int i = 5; i >= 0; i--)
			for (int i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					texFormat,
					width,
					height / 6,
					0,
					texFormat,
					GL_UNSIGNED_BYTE,
					(const GLubyte *)textureData[0] + width*(height / 6)*bpp*i);
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}


	TextureCube::TextureCube(const TextureInfo & textureInfo)
		:TextureCube(textureInfo.textureBuffer,textureInfo.width, textureInfo.height, textureInfo.bpp,
			textureInfo.isHDR)
	{
		for (auto &texture : textureInfo.textureBuffer) {
			FileSystem::getInstance().freeTexture(texture);
		}
	}

	TextureCube::TextureCube(const std::string & fileAndPath)
		: TextureCube(FileSystem::getInstance().loadTexture(fileAndPath, false)) //cubeTextures doesn't need uv flip for some weird reason
	{
	}

	void TextureCube::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	}
	void TextureCube::unBind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}