#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "RE\ReEngAppClass.h"
class Camera
{
public:
    void Initialize();
    static Camera* instance;
    static Camera* GetInstance();
    static void ReleaseInstance();
    glm::mat4 GetView();
    glm::mat4 GetProjection(bool bOrthographic);
    void SetPosition(glm::vec3 v3Position);
    void SetTarget(glm::vec3 v3Target);
    void SetUp(glm::vec3 v3Up);
    void MoveForward(float fIncrement);
    void MoveSideways(float fIncrement);
    void MoveVertical(float fIncrement);
    void ChangePitch(float fIncrement);
    void ChangeRoll(float fIncrement);
    void ChangeYaw(float fIncrement);

private:
    Camera();
    ~Camera();
    glm::mat4 camera;
    glm::quat rotation;
    glm::vec4 position;
    glm::vec4 target;
    glm::vec3 up;
    float fov, aspectRatio, nearDist, farDist;
};

#endif