#pragma once
#include "graphics/ITexture.h"

namespace Nova {
	class TextureCube : public ITexture
	{
	public:
		TextureCube() = delete;		
		//TextureCube(const std::array<TextureInfo, 6>& textureInfoArray);		
		TextureCube(const std::vector<void*>textureData, int width, int height, int bitsPerPixel, bool isHDR = false);
		TextureCube(const TextureInfo& textureInfo);
		TextureCube(const std::string &fileAndPath);
		~TextureCube();

		TextureCube(const TextureCube&) = delete;		 //copy ctor
		TextureCube(TextureCube&& other);						 //move ctor
		TextureCube& operator=(TextureCube const&) = delete;	 //copy assignment op
		TextureCube& operator=(TextureCube && other);		 //move assignment op

		void bind() const override;
		void unBind() const override;		
	private:
		GLuint mTextureID = 0;
		int mWidth;
		int mHeight;
		bool mHasMipmaps = false;
		TextureType mType;
		unsigned char * mTextureBuffer;
	};

}
