#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

#include "graphics/gl/Mesh.h"

class Camera {
public:
    Camera();
    virtual ~Camera();

    void draw(Mesh*, const GLuint&);
    void setDirection(double, double);
    void setLens(double, double, double, double);
    void setPosition(double, double, double);




protected:

private:
    glm::vec3 CameraPosition;
    glm::vec3 CameraDirection;
    glm::vec3 CameraLookAt;
    glm::vec3 CameraTop;

    glm::mat4 FinalMatrix;
    glm::mat4 PositionMatrix;
    glm::mat4 LensMatrix;

    float CameraFrustrumFieldOfView;
    float CameraFrustrumAspectRatio;
    float CameraFrustrumNearPlane;
    float CameraFrustrumFarPlane;

    GLint viewproj_location;
    GLint _currentProgram;

    bool lensDirty;
    bool positionDirty;

    void cleanFinalMatrix();
    void cleanLensMatrix();
    void cleanPositionMatrix();

    void findSlotOnGPU();
    void refreshIntoGPU();



};

#endif // CAMERA_H
