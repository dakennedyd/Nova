#pragma once
#include "graphics/IRenderPacket.h"
#include "graphics/opengl/RenderPacket.h"
//#include "graphics/opengl/Mesh.h"
#include "graphics/opengl/Material.h"

namespace Nova
{
class Mesh;
struct Transform;
class RenderPacket : public IRenderPacket
{
  public:
    RenderPacket() = delete;
    RenderPacket(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material,
                 const Transform *transform);
    RenderPacket(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Material> &material);
    ~RenderPacket();
    RenderPacket(const RenderPacket &) = delete;            // copy ctor
    RenderPacket(RenderPacket &&other);                     // move ctor
    RenderPacket &operator=(RenderPacket const &) = delete; // copy assignment op
    RenderPacket &operator=(RenderPacket &&other);          // move assignment op

    const std::shared_ptr<Material> &getMaterial() const { return mMaterial; };
    // void setMaterial(const std::shared_ptr<Material> material) { mMaterial = material; };
    const std::shared_ptr<Mesh> &getMesh() const { return mMesh; };

    void addParameter(IGPUProgramParameter *parameter) override
    {
        mParameters.push_back(parameter);
    };
    void updateAllUniforms() const override;
    void draw() const override;

    void bind() const override;
    void unBind() const override;

    // const std::uint64_t getID() const { return mID; };
  private:
    std::shared_ptr<Mesh> mMesh = nullptr;
    std::shared_ptr<Material> mMaterial = nullptr;
    // RenderPacket() {mID = mIDCounter; mIDCounter++; };
    const Transform *mTransform = nullptr;
    // const Mesh& mMesh;
    // const Material& mMaterial;
    std::vector<IGPUProgramParameter *> mParameters;
    // std::uint64_t mID;
    // static std::uint64_t mIDCounter;
};
} // namespace Nova
