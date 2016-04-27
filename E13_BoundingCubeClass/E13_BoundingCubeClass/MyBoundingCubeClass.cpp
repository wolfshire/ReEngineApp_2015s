#include "MyBoundingCubeClass.h"
//  MyBoundingCubeClass
void MyBoundingCubeClass::Init(void)
{
    m_fRadius = 0.0f;
    m_m4ToWorld = IDENTITY_M4;

    m_v3Center = vector3(0.0f);
    m_v3Min = vector3(0.0f);
    m_v3Max = vector3(0.0f);
}
void MyBoundingCubeClass::Swap(MyBoundingCubeClass& other)
{
    std::swap(m_fRadius, other.m_fRadius);
    std::swap(m_m4ToWorld, other.m_m4ToWorld);

    std::swap(m_v3Center, other.m_v3Center);
    std::swap(m_v3Min, other.m_v3Min);
    std::swap(m_v3Max, other.m_v3Max);
}
void MyBoundingCubeClass::Release(void)
{

}
//The big 3
MyBoundingCubeClass::MyBoundingCubeClass(std::vector<vector3> a_lVectorList)
{
    points = a_lVectorList;
    uint nVertexCount = a_lVectorList.size();

    if (nVertexCount > 0)
    {
        m_v3Min = a_lVectorList[0];
        m_v3Max = a_lVectorList[0];
    }

    for (uint i = 0; i < nVertexCount; i++)
    {
        if (a_lVectorList[i].x > m_v3Max.x)
            m_v3Max.x = a_lVectorList[i].x;
        else if (a_lVectorList[i].x < m_v3Min.x)
            m_v3Min.x = a_lVectorList[i].x;

        if (a_lVectorList[i].y > m_v3Max.y)
            m_v3Max.y = a_lVectorList[i].y;
        else if (a_lVectorList[i].y < m_v3Min.y)
            m_v3Min.y = a_lVectorList[i].y;

        if (a_lVectorList[i].z > m_v3Max.z)
            m_v3Max.z = a_lVectorList[i].z;
        else if (a_lVectorList[i].z < m_v3Min.z)
            m_v3Min.z = a_lVectorList[i].z;
    }

    m_v3Center = (m_v3Max + m_v3Min) / 2.0f;
    m_fRadius = glm::distance(m_v3Center, m_v3Max);
    m_v3Size.x = glm::distance(vector3(m_v3Min.x, 0.0, 0.0), vector3(m_v3Max.x, 0.0, 0.0));
    m_v3Size.y = glm::distance(vector3(0.0, m_v3Min.y, 0.0), vector3(0.0, m_v3Max.y, 0.0));
    m_v3Size.z = glm::distance(vector3(0.0f, 0.0, m_v3Min.z), vector3(0.0, 0.0, m_v3Max.z));
}
MyBoundingCubeClass::MyBoundingCubeClass(MyBoundingCubeClass const& other)
{
    m_fRadius = other.m_fRadius;
    m_m4ToWorld = other.m_m4ToWorld;

    m_v3Center = other.m_v3Center;
    m_v3Min = other.m_v3Min;
    m_v3Max = other.m_v3Max;
}
MyBoundingCubeClass& MyBoundingCubeClass::operator=(MyBoundingCubeClass const& other)
{
    if (this != &other)
    {
        Release();
        Init();
        MyBoundingCubeClass temp(other);
        Swap(temp);
    }
    return *this;
}

MyBoundingCubeClass::~MyBoundingCubeClass() { Release(); };

//Accessors
void MyBoundingCubeClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
    m_m4ToWorld = a_m4ToWorld;
    /*
    vector3 p[] = 
    {
        m_v3Max,
        vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z),
        vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z),
        vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z),
        m_v3Min,
        vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z),
        vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z),
        vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z)
    };

    min = max = p[0];

    for (int i = 0; i < 8; i++)
    {
        vector4 point = vector4(p[i], 1.0f) * glm::mat4_cast(glm::quat(m_m4ToWorld));

        if (i == 0)
            min = max = vector3(point.x, point.y, point.z);

        if (point.x > max.x) max.x = point.x;
        if (point.y > max.y) max.y = point.y;
        if (point.z > max.z) max.z = point.z;
        if (point.x < min.x) min.x = point.x;
        if (point.y < min.y) min.y = point.y;
        if (point.z < min.z) min.z = point.z;
    }*/
    
    for (uint i = 0; i < points.size(); i++)
    {
        vector3 point = vector3(m_m4ToWorld * vector4(points[i], 1.0f));

        if (i == 0)
            min = max = vector3(point.x, point.y, point.z);

        if (point.x > max.x)
            max.x = point.x;
        else if (point.x < min.x)
            min.x = point.x;

        if (point.y > max.y)
            max.y = point.y;
        else if (point.y < min.y)
            min.y = point.y;

        if (point.z > max.z)
            max.z = point.z;
        else if (point.z < min.z)
            min.z = point.z;
    }
    
    float tmp;

    if (max.x < min.x)
    {
        tmp = min.x;
        min.x = max.x;
        max.x = tmp;
    }
    if (max.y < min.y)
    {
        tmp = min.y;
        min.y = max.y;
        max.y = tmp;
    }
    if (max.z < min.z)
    {
        tmp = min.z;
        min.z = max.z;
        max.z = tmp;
    }

    center = (max + min) / 2.0f;
    //m_fRadius = glm::distance(m_v3Center, max);
    size.x = glm::distance(vector3(min.x, 0.0, 0.0), vector3(max.x, 0.0, 0.0));
    size.y = glm::distance(vector3(0.0, min.y, 0.0), vector3(0.0, max.y, 0.0));
    size.z = glm::distance(vector3(0.0f, 0.0, min.z), vector3(0.0, 0.0, max.z));
}

vector3 MyBoundingCubeClass::GetCenterG(void) { return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }

float MyBoundingCubeClass::GetRadius(void) { return m_fRadius; }

vector3 MyBoundingCubeClass::GetSize(void) { return m_v3Size; };

//--- Non Standard Singleton Methods
bool MyBoundingCubeClass::IsColliding(MyBoundingCubeClass* const a_pOther)
{
    //Collision check goes here
    vector3 v3Temp = vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f));
    vector3 v3Temp1 = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Center, 1.0f));

    bool bAreColliding = true;
    vector3 vMin1 = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
    vector3 vMax1 = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));
    vector3 vMin2 = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Min, 1.0f));
    vector3 vMax2 = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Max, 1.0f));

    //Check for X
    if (vMax1.x < vMin2.x)
        bAreColliding = false;
    if (vMin1.x > vMax2.x)
        bAreColliding = false;

    //Check for Y
    if (vMax1.y < vMin2.y)
        bAreColliding = false;
    if (vMin1.y > vMax2.y)
        bAreColliding = false;

    //Check for Z
    if (vMax1.z < vMin2.z)
        bAreColliding = false;
    if (vMin1.z > vMax2.z)
        bAreColliding = false;

    return bAreColliding;
}
matrix4 MyBoundingCubeClass::GetModelMatrix(void) { return m_m4ToWorld; }