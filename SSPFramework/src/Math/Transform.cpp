#include "pch.h"
#include "Transform.h"
#include "Log.h"

void Transform::Translate(Math::vec3 pos)
{
    m_position.x += pos.x;
    m_position.y += pos.y;
    m_position.z += pos.z;
}
void Transform::SetPosition(Math::vec3 pos)
{
    m_position.x = pos.x;
    m_position.y = pos.y;
    m_position.z = pos.z;
}
Math::vec3 Transform::GetPosition()
{
    return m_position;
}
void Transform::Scale(Math::vec3 scale)
{
    m_scale *= scale;
}
void Transform::SetScale(Math::vec3 scale)
{
    m_scale.x = scale.x;
    m_scale.y = scale.y;
    m_scale.z = scale.z;
}
Math::vec3 Transform::GetScale()
{
    return m_scale;
}
void Transform::SetRotation(Math::vec3 rot)
{
    // Set pitchYawRoll
    m_pitchYawRoll = rot;

    // Set Quaternion rotation
    m_rotation = Math::quat(Math::vec3(glm::radians(m_pitchYawRoll.x), glm::radians(m_pitchYawRoll.y), glm::radians(m_pitchYawRoll.z)));

    // Set forward vector
    m_forward = Math::normalize(m_rotation * Math::vec3(0, 0, 1));

    // Set right vector
    m_right = Math::normalize(m_rotation * Math::vec3(1, 0, 0));

    // Set up vector
    m_up = Math::normalize(m_rotation * Math::vec3(0, 1, 0));
}
void Transform::SetRotation(Math::quat rot)
{
    // Set quaternion
    m_rotation = rot;

    // Set forward vector
    m_forward = Math::normalize(m_rotation * Math::vec3(0, 0, 1));

    // Set right vector
    m_right = Math::normalize(m_rotation * Math::vec3(1, 0, 0));

    // Set up vector
    m_up = Math::normalize(m_rotation * Math::vec3(0, 1, 0));

    // Set pitch yaw roll
    m_pitchYawRoll.x = Math::degrees(std::asinf(m_forward.y));
    m_pitchYawRoll.y = Math::degrees(std::atan2f(-m_forward.x, m_forward.z));
    m_pitchYawRoll.z = Math::degrees(std::atan2f(-m_right.y, m_up.y));
}
Math::vec3 Transform::GetPitchYawRoll()
{
    return m_pitchYawRoll;
}
Math::quat Transform::GetRotation()
{
    return m_rotation;
}
void Transform::Rotate(Math::vec3 rot)
{
    // Rotate pitchYawRoll by a value
    m_pitchYawRoll += rot;

    // Set Quaternion rotation
    m_rotation = Math::quat(Math::vec3(glm::radians(m_pitchYawRoll.x), glm::radians(m_pitchYawRoll.y), glm::radians(m_pitchYawRoll.z)));

    // Set forward vector
    m_forward = Math::normalize(m_rotation * Math::vec3(0, 0, 1));

    // Set right vector
    m_right = Math::normalize(m_rotation * Math::vec3(1, 0, 0));

    // Set up vector
    m_up = Math::normalize(m_rotation * Math::vec3(0, 1, 0));
}
Math::vec3 Transform::GetForward()
{
    return m_forward;
}
Math::vec3 Transform::GetRight()
{
    return m_right;
}
Math::vec3 Transform::GetUp()
{
    return m_up;
}