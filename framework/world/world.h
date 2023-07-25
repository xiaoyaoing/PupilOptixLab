#pragma once

#include "camera.h"
#include "emitter.h"
#include "ias_manager.h"
#include "render_object.h"

#include "resource/scene.h"
#include "util/util.h"

#include <filesystem>
#include <memory>

namespace Pupil {
enum class EWorldEvent {
    CameraChange,
    CameraMove,
    CameraFovChange,
    CameraViewChange,
    RenderInstanceUpdate
};

namespace world {
class World : public util::Singleton<World> {
public:
    std::unique_ptr<resource::Scene> scene = nullptr;
    std::unique_ptr<CameraHelper> camera = nullptr;
    std::unique_ptr<EmitterHelper> emitters = nullptr;

    void Init() noexcept;
    void Destroy() noexcept;

    bool LoadScene(std::filesystem::path) noexcept;
    bool LoadScene(resource::Scene *) noexcept;

    OptixTraversableHandle GetIASHandle(unsigned int gas_offset = 2, bool allow_update = false) noexcept;

    RenderObject *GetRenderObject(std::string_view id) const noexcept;
    RenderObject *GetRenderObject(size_t index) const noexcept;

    void UpdateRenderObject(RenderObject *) noexcept;

    std::vector<RenderObject *> GetRenderobjects() noexcept;

    void SetDirty() noexcept;
    bool IsDirty() const noexcept;
    void SetDirty(unsigned int gas_offset, bool allow_update) noexcept;
    bool IsDirty(unsigned int gas_offset, bool allow_update) const noexcept;

    util::Camera &GetUtilCamera() noexcept { return camera->GetUtilCamera(); }
    optix::Camera &GetOptixCamera() noexcept { return camera->GetOptixCamera(); }

    util::AABB GetAABB() noexcept;

private:
    std::vector<std::unique_ptr<RenderObject>> m_ros;
    std::unique_ptr<IASManager> m_ias_manager;
};
}// namespace world
}// namespace Pupil