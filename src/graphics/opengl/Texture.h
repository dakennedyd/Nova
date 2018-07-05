#pragma once
#include "graphics/ITexture.h"

namespace Nova {
	enum MipMapping {NO_MIPMAP, GENERATE_MIPMAP};
	enum Filtering { LINEAR, NEAREST };
	enum TextureType {COLOR, COLOR_ALPHA, DEPTHBUFFER, DEPTHSTENCIL, GBUFFER_NORMAL, GBUFFER_POSITION,
	MONOCHROME, DUAL_COLOR, NORMAL_MAP, COLOR_HDR};

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
		bool mHasMipmaps = false;
		TextureType mType;
		unsigned char * mTextureBuffer;

	};
}
