#pragma once
#include <glad/glad.h>
#include "graphics/ITexture.h"
#include "PreInit.h"
#ifdef NOVA_WINDOWS_PLATFORM
	#include "windows/FileSystem.h"
#elif defined  NOVA_LINUX_PLATFORM
	#include "linux/FileSystem.h"
#endif

namespace Nova {	
	class Texture : public ITexture
	{
		friend class FrameBuffer;
	public:
		Texture() = delete;		
		//filtering option only is relevant for color textures(COLOR, COLOR_ALPHA) the rest are all NEAREST
		Texture(const int width, const int height, const void *textureData = nullptr,
			const enum TextureType &textureType = TextureType::COLOR, const enum Filtering &filtering = Filtering::LINEAR,
			const enum MipMapping &mipmap = MipMapping::GENERATE_MIPMAP);
		Texture(const std::string& fileAndPath);

		~Texture();
		Texture(const Texture&) = delete;		 //copy ctor
		Texture(Texture&& other);						 //move ctor
		Texture& operator=(Texture const&) = delete;	 //copy assignment op
		Texture& operator=(Texture && other);		 //move assignment op

		void bind() const override;
		void unBind() const override;

		//binds and unbinds the texture
		void setFiltering(const enum Filtering &option);		
		GLuint getTextureID() { return mTextureID; }
		TextureType getTextureType() { return mType; }

		static Texture makeShadowMap(const int width, const int height)
		{
			return Texture(width, height, nullptr, TextureType::DEPTHBUFFER);
		}

		int getWidth() { return mWidth; }
		int getHeight() { return mHeight; }
	private:
		Texture(const TextureInfo& textureInfo);
		GLuint mTextureID;		//opengl texture handle 
		float mBorderColor[4] = {0.0f,0.0f,0.0f,0.0f};
		//TextureState mState;		
		int mWidth;
		int mHeight;
		TextureType mType;
		bool mHasMipmaps = false;
		unsigned char * mTextureBuffer;

	};
}
