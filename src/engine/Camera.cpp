#include "engine/Camera.hpp"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace cheatbase::engine {

core::Matrix4x4 Camera::CreateViewMatrix(const core::Vector3& loc, const core::Vector3& rot) {
    core::Matrix4x4 matrix;
    
    float pitch = rot.x * (M_PI / 180.0f);
    float yaw = rot.y * (M_PI / 180.0f);
    float roll = rot.z * (M_PI / 180.0f);

    float SP = std::sin(pitch);
    float CP = std::cos(pitch);
    float SY = std::sin(yaw);
    float CY = std::cos(yaw);
    float SR = std::sin(roll);
    float CR = std::cos(roll);

    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.0f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.0f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.0f;

    matrix.m[3][0] = loc.x;
    matrix.m[3][1] = loc.y;
    matrix.m[3][2] = loc.z;
    matrix.m[3][3] = 1.0f;

    return matrix;
}

core::Matrix4x4 Camera::CreateProjectionMatrix(float fov, float aspect) {
    core::Matrix4x4 matrix;
    float halfFov = (fov / 2.0f) * (M_PI / 180.0f);
    float yRange = 1.0f / std::tan(halfFov);
    float xRange = yRange / aspect;

    matrix.m[0][0] = xRange;
    matrix.m[1][1] = yRange;
    matrix.m[2][2] = 0.0f;
    matrix.m[2][3] = 1.0f;
    matrix.m[3][2] = 10.0f; // zNear (approximation for UE projection)
    
    return matrix;
}

core::Matrix4x4 Camera::MultiplyMatrix(const core::Matrix4x4& m1, const core::Matrix4x4& m2) {
    core::Matrix4x4 result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = m1.m[i][0] * m2.m[0][j] +
                             m1.m[i][1] * m2.m[1][j] +
                             m1.m[i][2] * m2.m[2][j] +
                             m1.m[i][3] * m2.m[3][j];
        }
    }
    return result;
}

bool Camera::WorldToScreen(const core::Vector3& worldLoc, const core::Vector3& camLoc, const core::Vector3& camRot, float fov, core::Vector2 screenSize, core::Vector2& outScreenLoc) {
    core::Matrix4x4 viewMatrix = CreateViewMatrix(camLoc, camRot);
    core::Matrix4x4 projMatrix = CreateProjectionMatrix(fov, screenSize.x / screenSize.y);
    core::Matrix4x4 viewProjMatrix = MultiplyMatrix(viewMatrix, projMatrix);

    float w = viewProjMatrix.m[0][3] * worldLoc.x + viewProjMatrix.m[1][3] * worldLoc.y + viewProjMatrix.m[2][3] * worldLoc.z + viewProjMatrix.m[3][3];

    if (w < 0.1f) return false;

    float x = viewProjMatrix.m[0][0] * worldLoc.x + viewProjMatrix.m[1][0] * worldLoc.y + viewProjMatrix.m[2][0] * worldLoc.z + viewProjMatrix.m[3][0];
    float y = viewProjMatrix.m[0][1] * worldLoc.x + viewProjMatrix.m[1][1] * worldLoc.y + viewProjMatrix.m[2][1] * worldLoc.z + viewProjMatrix.m[3][1];

    core::Vector2 ndc(x / w, y / w);
    
    outScreenLoc.x = (screenSize.x / 2.0f) * (1.0f + ndc.x);
    outScreenLoc.y = (screenSize.y / 2.0f) * (1.0f - ndc.y); // Y is flipped in screen space

    return true;
}

} // namespace cheatbase::engine
