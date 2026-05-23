#pragma once

#include "core/Math.hpp"

namespace cheatbase::engine {

struct FTransform {
    core::Vector4 Rotation;
    core::Vector3 Translation;
    core::Vector3 Scale3D;
};

class Camera {
public:
    static bool WorldToScreen(const core::Vector3& worldLocation, 
                              const core::Vector3& cameraLocation, 
                              const core::Vector3& cameraRotation, 
                              float fov, 
                              core::Vector2 screenSize, 
                              core::Vector2& outScreenLocation);

private:
    static core::Matrix4x4 CreateViewMatrix(const core::Vector3& cameraLocation, const core::Vector3& cameraRotation);
    static core::Matrix4x4 CreateProjectionMatrix(float fov, float aspect);
    static core::Matrix4x4 MultiplyMatrix(const core::Matrix4x4& m1, const core::Matrix4x4& m2);
};

} // namespace cheatbase::engine
