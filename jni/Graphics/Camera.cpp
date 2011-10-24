#include "Camera.h"
#include <cmath>
#include <Utils/Log.h>

Camera::Camera()
{
    _position = Vector3(0,0,0);
    _direction = Vector3(0, 0, -1);
    _upVector = Vector3(0,1,0);
    _leftVector=_direction.CrossProduct(_upVector);
    _horizontalAngle = 0.0f;
    _verticalAngle = 0.0f;
    _projectionMatrix.LoadIdentity();
}


void Camera::SetProjection(float fovy, float aspect, float near, float far) {
    _projectionMatrix.LoadIdentity();
    _projectionMatrix.SetPerspective(fovy, aspect, near, far);
}

void Camera::Update() {
    _viewMatrix.LoadIdentity();

    Vector3 directionPoint = _position + _direction;



    Vector3 F = Vector3(directionPoint.x - _position.x,
                        directionPoint.y - _position.y,
                        directionPoint.z - _position.z);
    F.Normalize();

    Vector3 UP = _upVector;

    UP.Normalize();

    Vector3 s = F.CrossProduct(UP);

    Vector3 u = s.CrossProduct(F);


    _viewMatrix.SetTranslationPart(-_position);


    Matrix4x4 rotateVert;
    rotateVert.SetRotationX(_verticalAngle*180.0f/PI);

    Matrix4x4 rotateHor;
    rotateHor.SetRotationY(_horizontalAngle*180.0f/PI);

   _viewMatrix = _viewMatrix*rotateVert*rotateHor;


    /*glPushMatrix();
    glLoadIdentity();

    Vector3 directionPoint = _position + _direction;

    gluLookAt(_position.x, _position.y, _position.z,
              directionPoint.x, directionPoint.y, directionPoint.z,
              _upVector.x, _upVector.y, _upVector.z);

    glRotatef(_verticalAngle*180/PI, 1,0,0);
    glRotatef(_horizontalAngle*180/PI, 0,1,0);

    glGetFloatv(GL_MODELVIEW_MATRIX, &_viewMatrix.entries[0]);

    glPopMatrix();*/
}

Matrix4x4 Camera::GetViewMatrix() const {
    return _viewMatrix;
}


Matrix4x4 Camera::GetProjectionMatrix() const {
    return _projectionMatrix;
}

void Camera::SetDirection(Vector3 direction) {
    _direction = direction;
}

void Camera::SetPosition(Vector3 position) {
    _position = position;
}

void Camera::SetUpVector(Vector3 upVector) {
    _upVector = upVector;
}

Vector3 Camera::GetDirection() const {
    return _direction;
}

Vector3 Camera::GetPosition() const {
    return _position;
}

Vector3 Camera::GetUpVector() const {
    return _upVector;
}

Vector3 Camera::GetLeftVector() const {
    return _leftVector;
}

float Camera::GetHorizontalAngle() const {
    return _horizontalAngle;
}

float Camera::GetVerticalAngle() const {
    return _verticalAngle;
}

void Camera::SetHorizontalAngle(float horizontalAngle) {
    _horizontalAngle = horizontalAngle;
}

void Camera::SetVerticalAngle(float verticalAngle) {
    _verticalAngle = verticalAngle;
}

void Camera::MoveFoward(float distance) {
    _position += _direction*distance;
}

void Camera::MoveLeft(float distance) {
    _position += _leftVector*distance;
}

void Camera::MoveUp(float distance) {
    _position += _upVector*distance;
}

void Camera::RotateLeft(float rotation) {
    _horizontalAngle += rotation;
}

void Camera::RotateUp(float rotation) {
    _verticalAngle += rotation;
}


void Camera::Clone(const Camera *c) {
    _position = c->GetPosition();
    _direction = c->GetDirection();
    _upVector = c->GetUpVector();
    _leftVector = c->GetLeftVector();
    _horizontalAngle = c->GetHorizontalAngle();
    _verticalAngle = c->GetVerticalAngle();
    _viewMatrix = c->GetViewMatrix();
    _projectionMatrix = c->GetProjectionMatrix();
}