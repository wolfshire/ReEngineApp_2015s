#include "camera.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
    //initialize view/rotation
    Initialize();
}

void Camera::Initialize()
{
    fov = 65.0f;
    aspectRatio = 4.0f / 3.0f;
    nearDist = 0.1f;
    farDist = 1000.0f;
    position = vector4(0.0f, 0.0f, -10.0f, 1.0f);
    up = vector3(0.0f, 1.0f, 0.0f);
    target = vector4(0.0f, 0.0f, 0.0f, 1.0f);
    rotation = glm::quat();
}

Camera::~Camera(){}

Camera* Camera::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Camera();
    }
    return instance;
}

void Camera::ReleaseInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

glm::mat4 Camera::GetProjection(bool bOrthographic)
{
    if (bOrthographic)
        return glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f);
    else
        return glm::perspective(fov, aspectRatio, nearDist, farDist);
}

glm::mat4 Camera::GetView()
{
    return glm::lookAt(vector3(position.x, position.y, position.z), vector3(target.x, target.y, target.z), up);
}

void Camera::SetPosition(glm::vec3 pos)
{
    position = vector4(pos, 1.0f);
}

void Camera::SetTarget(glm::vec3 tar)
{
    target = vector4(tar, 1.0f);
}

void Camera::SetUp(glm::vec3 u)
{
    up = u;
}

void Camera::MoveForward(float fIncrement)
{
    position += (fIncrement * glm::normalize(vector4(0.0f,0.0f,1.0f,1.0f) * rotation));
    target += (fIncrement * glm::normalize(vector4(0.0f, 0.0f, 1.0f, 1.0f) * rotation));
}
    
void Camera::MoveSideways(float fIncrement)
{
    position += (fIncrement * glm::normalize(vector4(1.0f, 0.0f, 0.0f, 1.0f) * rotation));
    target += (fIncrement * glm::normalize(vector4(1.0f, 0.0f, 0.0f, 1.0f) * rotation));
}

void Camera::MoveVertical(float fIncrement)
{
    position += fIncrement * glm::normalize(vector4(0.0f, 1.0f, 0.0f, 1.0f) * rotation);
    target += fIncrement * glm::normalize(vector4(0.0f, 1.0f, 0.0f, 1.0f) * rotation);
}

void Camera::ChangePitch(float fIncrement) //x
{
    fIncrement *= 10;
    //rotate
    rotation = glm::rotate(rotation, fIncrement, vector3(1, 0, 0));
	//set the up/forward vector
	vector4 newUp = position + (vector4(0.0f, 1.0f, 0.0f, 0.0f) * rotation);
	up = vector3(newUp.x, newUp.y, newUp.z);
	target = position * glm::cross(quaternion(0.0f, 0.0f, 1.0f, 0.0f), rotation);
}

void Camera::ChangeRoll(float fIncrement) //z
{
    fIncrement *= 10;
    //rotate
    rotation = glm::rotate(rotation, fIncrement, vector3(0, 0, 1));
    //set the up/forward vector
	vector4 newUp = position + (vector4(0.0f, 1.0f, 0.0f, 0.0f) * rotation);
	up = vector3(newUp.x, newUp.y, newUp.z);
	target = position * glm::cross(quaternion(0.0f, 0.0f, 1.0f, 0.0f),rotation);

}

void Camera::ChangeYaw(float fIncrement) //y
{
    fIncrement *= 10;
    //rotate
    rotation = glm::rotate(rotation, fIncrement, vector3(0, 1, 0));
	//set the up/forward vector
	vector4 newUp = position + (vector4(0.0f, 1.0f, 0.0f, 0.0f) * rotation);
	up = vector3(newUp.x, newUp.y, newUp.z);
	target = position * glm::cross(quaternion(0.0f, 0.0f, 1.0f, 0.0f), rotation);
}
