#pragma once

namespace Nova {	
	class UnitQuat;

	class Vec3
	{			
		friend class Mat3;
		friend class UnitQuat;
	public:
		Vec3();
		Vec3(const float value);
		Vec3(const float x, const float y, const float z);
		~Vec3() = default;				
		friend Vec3 operator+(const Vec3& left, const Vec3& right);		
		friend Vec3 operator-(const Vec3& left, const Vec3& right);		
		friend float operator*(const Vec3& left, const Vec3& right);	// scalar/dot product		
		friend Vec3 operator*(const float value, const Vec3& left);
		friend Vec3 operator/(const float value, const Vec3& left);
		Vec3 operator*(const float value) const;
		Vec3 operator/(const float value) const;
		//Vec3 operator*(const Mat3& right) const;
		friend Vec3 operator^(const Vec3& left, const Vec3& right);		// vector/cross product				

		const float * getDataPtr() const;

		/** Rotate a vector around rotAxis uses Rodriguez formula
		*	rotAxis must be normalized otherwize results are undefined
		*/
		void rotateSelf(const Vec3& rotAxis, const float angle);
		void rotateSelf(const UnitQuat& q);
		Vec3 rotate(const UnitQuat& q) const;
		Vec3 rotate(const Vec3& rotAxis, const float angle) const;

		float getX() const;
		float getY() const;
		float getZ() const;
		void setX(const float value);
		void setY(const float value);
		void setZ(const float value);

		float getMagnitud();

		/** return the euclidian distance between 2 points */
		float getDistance(const Vec3& v);
		void normalizeSelf();
		Vec3 normalize() const;
		bool isNormalized() const;
		void debugPrint() const;
	private:
		float mData[3];		
		float mMagnitud = -1.0f;						
	};

	class Vec4
	{
		friend class Mat4;
		friend class UnitQuat;
	public:
		Vec4();
		Vec4(const float value);
		Vec4(const float x, const float y, const float z, const float w);
		Vec4(const Vec3 &vec, const float w = 1.0f);
		~Vec4() = default;		
		friend Vec4 operator+(const Vec4& left, const Vec4& right);		
		friend Vec4 operator-(const Vec4& left, const Vec4& right);		
		friend float operator*(const Vec4& left, const Vec4& right);		

		const float * getDataPtr() const;

		float getX() const;
		float getY() const;
		float getZ() const;
		float getW() const;
		void setX(const float value);
		void setY(const float value);
		void setZ(const float value);
		void setW(const float value);

		Vec4 rotate(const UnitQuat& q) const;

		float getMagnitud();
		void normalizeSelf();
		void debugPrint() const;
	private:
		float mData[4];
		float mMagnitud = -1.0f;			
	};
}
