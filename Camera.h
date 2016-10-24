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

#include "src/graphics/gl/Mesh.h"
#include "src/I_Moveable.h"

class Camera {
public:
    Camera();
    virtual ~Camera();

    void attachTo( I_Moveable* );
    void draw( Mesh*, const GLuint& );
    void setLens( double, double, double, double );




protected:

private:

    I_Moveable* attachee;

    float CameraFrustrumFieldOfView;
    float CameraFrustrumAspectRatio;
    float CameraFrustrumNearPlane;
    float CameraFrustrumFarPlane;

    glm::vec3 CameraPosition;
    glm::vec3 CameraDirection;
    glm::vec3 CameraLookAt;
    glm::vec3 CameraTop;

    glm::mat4 FinalMatrix;
    glm::mat4 LensMatrix;
    glm::mat4 PositionMatrix;

    GLint viewproj_location;
    GLint _currentProgram;

    bool lensDirty;
    bool positionDirty;

    void cleanFinalMatrix();
    void cleanLensMatrix();
    void cleanPositionMatrix();

    void findSlotOnGPU();
    void refreshIntoGPU();

    void setDirection( double, double );
    void setPosition( double, double, double );
};

#endif // CAMERA_H
