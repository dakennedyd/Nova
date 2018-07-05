#pragma once
#include "graphics/ILight.h"

namespace Nova {
	class Light :public ILight		
	{
	public:
		Light() = delete;
		Light(int type, Vec3 *pos, Vec3 *color);//, Vec3 *amb, Vec3 *diff, Vec3 *spec);
		~Light() = default;
		Light(const Light&) = delete;		 //copy ctor
		Light(Light&& other);						 //move ctor
		Light& operator=(Light const&) = delete;	 //copy assignment op
		Light& operator=(Light && other);		 //move assignment op

		const Vec3 *getPosition() const { return mPosition; };
		const Vec3 *getColor() const { return mColor; };
		/*const Vec3 *getAmbientColor() const;
		const Vec3 *getDiffuseColor() const;
		const Vec3 *getSpecularColor() const;*/
		const int getTypeCode() const { return mType; };

		//const std::uint64_t getID() const { return mID; };
		/*float mConstant = 1.0f;
		float mLinear = 0.7f;
		float mQuadratic = 1.1f;*/
		
	private:
		/*Vec3 *mPosition;
		Vec3 *mAmbient;
		Vec3 *mDiffuse;
		Vec3 *mSpecular;*/

		Vec3 *mPosition;
		Vec3 *mColor;
		int mType;
		//std::uint64_t mID;
		//static std::uint64_t mIDCounter;
	};
}
