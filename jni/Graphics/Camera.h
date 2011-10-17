#ifndef CAMERA_H
#define CAMERA_H
#include <Math/MathLib.h>


#define PI 3.14156

class Camera
{
public:
    Camera();

    Vector3 GetPosition() const;
    Vector3 GetDirection() const;
    Vector3 GetUpVector() const;
    Vector3 GetLeftVector() const;

    Matrix4x4 GetViewMatrix() const;
    Matrix4x4 GetProjectionMatrix() const;

    void Clone(const Camera* c);

    void SetPosition(Vector3 position);
    void SetDirection(Vector3 direction);
    void SetUpVector(Vector3 upVector);

    void Update();
    void SetHorizontalAngle(float horizontalAngle);
    float GetHorizontalAngle() const;

    void SetVerticalAngle(float verticalAngle);
    float GetVerticalAngle() const;

    void MoveForvard(float distance);
    void MoveLeft(float distance);
    void MoveUp(float distance);

    void RotateUp(float rotation);
    void RotateLeft(float rotation);

    void SetProjection(float fovy, float aspect, float near, float far);

private:
    Vector3 _position;
    Vector3 _direction;
    Vector3 _upVector;
    Vector3 _leftVector;
    float _horizontalAngle;
    float _verticalAngle;
    Matrix4x4 _viewMatrix;
    Matrix4x4 _projectionMatrix;

};


#endif // CAMERA_H
