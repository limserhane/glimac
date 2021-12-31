#include "glimac/TrackballCamera.hpp"

TrackballCamera::TrackballCamera():
    m_fDistance(5),
    m_fAngleX(0),
    m_fAngleY(0)
{}


  
void TrackballCamera::moveFront(float delta)
{
    m_fDistance -= delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_fAngleY += degrees;
}

void TrackballCamera::rotateUp(float degrees)
{
    m_fAngleX += degrees;
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 viewMatrix = glm::mat4(1.f);

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.f, - m_fDistance));

    viewMatrix = glm::rotate(viewMatrix, - glm::radians(m_fAngleX), glm::vec3(1.f, 0.f, 0.f));
    viewMatrix = glm::rotate(viewMatrix, - glm::radians(m_fAngleY), glm::vec3(0.f, 1.f, 0.f));

    return viewMatrix;
}