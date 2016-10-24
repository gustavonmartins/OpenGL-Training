#include "Camera.h"
#include <iostream>

Camera::Camera():
    viewproj_location(-1),
    _currentProgram(-1),
    lensDirty(true),
    positionDirty(true),
    attachee(nullptr) {

}


Camera::~Camera() {
    //dtor
}

void Camera::attachTo(I_Moveable* newAttachee){
    if (nullptr!=newAttachee){attachee=newAttachee;}
}

void Camera::draw(Mesh* meshToDraw, const GLuint& renderProgram) {
    static bool programIsValid = false;
    if (_currentProgram != renderProgram) {

        if (GL_TRUE == glIsProgram(renderProgram)) {
            programIsValid = true;
            _currentProgram = renderProgram;
            findSlotOnGPU();
        }
    }
    if (programIsValid) {
        setPosition(attachee->getPosition().axis_one,attachee->getPosition().axis_two,attachee->getPosition().axis_three);
        setDirection(attachee->getOrientation().axis_one,attachee->getOrientation().axis_two);
        refreshIntoGPU();
        meshToDraw->draw(_currentProgram);
    }
}

void Camera::findSlotOnGPU() {
    std::cout << "_currentProgram on----: " << _currentProgram << std::endl;
    if (-1 != _currentProgram) {
        viewproj_location = glGetUniformLocation(_currentProgram, "ViewProjection_Matrix"); // The search field is as described in shader code}
        std::cout << "Camera viewproj on: " << viewproj_location << std::endl;
    }
}

void Camera::refreshIntoGPU() {
    cleanFinalMatrix();
    glUniformMatrix4fv(viewproj_location, 1, GL_FALSE, &FinalMatrix[0][0]);
}

void Camera::setDirection(double heading, double pitch) {

    if (pitch > -90 && pitch < 90) {CameraTop = glm::vec3(0.0f, 1.0f, 0.0f);}
    else {CameraTop = glm::vec3(0.0f, 1.0f, 0.0f);}

    CameraDirection = glm::vec3(-1.0f * (float)cos(glm::radians(heading)), 1.0f * (float)cos(glm::radians(pitch)), -1.0f * (float)sin(glm::radians(heading)));
    CameraLookAt = CameraPosition + CameraDirection;

    positionDirty = true;
}

void Camera::setLens(double FOV_deg, double AspectRatio, double NearPlane, double FarPlane) {

    CameraFrustrumFieldOfView = glm::radians(FOV_deg);
    CameraFrustrumAspectRatio = AspectRatio;
    CameraFrustrumNearPlane = NearPlane;
    CameraFrustrumFarPlane = FarPlane;

    lensDirty = true;

}

void Camera::setPosition(double myX, double myY, double myZ) {

    CameraPosition = glm::vec3(-myX, myZ, -myY);
    CameraLookAt = CameraPosition + CameraDirection;

    positionDirty = true;

}

void Camera::cleanFinalMatrix() {

    if (lensDirty || positionDirty) {
        if (lensDirty) {cleanLensMatrix();}
        if (positionDirty) {cleanPositionMatrix();}

        FinalMatrix = LensMatrix * PositionMatrix;
        lensDirty = false;
        positionDirty = false;
    }
}

void Camera::cleanLensMatrix() {
    LensMatrix = glm::perspective(CameraFrustrumFieldOfView, CameraFrustrumAspectRatio, CameraFrustrumNearPlane, CameraFrustrumFarPlane);
}

void Camera::cleanPositionMatrix() {
    PositionMatrix = glm::lookAt(CameraPosition, CameraLookAt, CameraTop);
}
