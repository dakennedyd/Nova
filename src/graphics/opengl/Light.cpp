#include "Light.h"
#include "math/Vector.h"
#include <utility>

namespace Nova
{
// std::uint64_t Light::mIDCounter = 0;
Light::Light(int type, Vec3 *pos, Vec3 *color) //, Vec3 * amb, Vec3 * diff, Vec3 * spec)
    : mPosition(pos), mColor(color), mType(type)
{
    // mID = mIDCounter;
    // mIDCounter++;
}
Light::Light(Light &&other)
    : mPosition(std::move(other.mPosition)), mColor(std::move(other.mColor)),
      mType(std::move(other.mType))
/*mAmbient(std::move(other.mAmbient)),
mDiffuse(std::move(other.mDiffuse)), mSpecular(std::move(other.mSpecular)),
mConstant(std::move(other.mConstant)), mLinear(std::move(other.mLinear)),
mQuadratic(std::move(other.mQuadratic))*/
// mID(std::move(other.mID))
{
}
Light &Light::operator=(Light &&other)
{
    if (&other != this)
    {
        mType = std::move(other.mType);
        mPosition = std::move(other.mPosition);
        mColor = std::move(other.mColor);
        /*mAmbient = std::move(other.mAmbient);
        mDiffuse = std::move(other.mDiffuse);
        mSpecular = std::move(other.mSpecular);
        mConstant = std::move(other.mConstant);
        mLinear = std::move(other.mLinear);
        mQuadratic = std::move(other.mQuadratic);*/
        // mID = std::move(other.mID);
    }
    return *this;
}
/*const Vec3 * Light::getAmbientColor() const
{
        return mAmbient;
}
const Vec3 * Light::getDiffuseColor() const
{
        return mDiffuse;
}
const Vec3 * Light::getSpecularColor() const
{
        return mSpecular;
}*/
} // namespace Nova