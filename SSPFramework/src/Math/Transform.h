#pragma once
#include "Math.h"

class Transform
{
public:
    //////////////////////////////////////////////////////////////////
    // Move by specified amount
    void Translate(Math::vec3 pos);
    // Set Position
    void SetPosition(Math::vec3 pos);
    // Get Position
    Math::vec3 GetPosition();
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    // Multiplies by specified amount
    void Scale(Math::vec3 scale);
    // Set Scale
    void SetScale(Math::vec3 scale);
    // Get Scale
    Math::vec3 GetScale();
    //////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////
    // Rotate
    void Rotate(Math::vec3 rot);
    // Set PitchYawRoll Rotation
    void SetRotation(Math::vec3 rot);
    // Get PitchYawRoll Rotation
    Math::vec3 GetPitchYawRoll();
    // Set Quaternion Rotation
    void SetRotation(Math::quat rot);
    // Get Quaternion Rotation
    Math::quat GetRotation();

    // Get Forward Vector
    Math::vec3 GetForward();
    // Get Right Vector
    Math::vec3 GetRight();
    // Get Up Vector
    Math::vec3 GetUp();
    //////////////////////////////////////////////////////////////////

    Math::vec3 m_position{ 0, 0, 0 };

    Math::vec3 m_scale{ 1, 1, 1 };

    Math::vec3 m_pitchYawRoll{ 0, 0, 0 };
    
    Math::quat m_rotation{ 0, 0, 0, 0 };

    Math::vec3 m_forward{0, 0, 1};
    Math::vec3 m_right{ 1, 0, 0 };
    Math::vec3 m_up{ 0, 1, 0 };
};

