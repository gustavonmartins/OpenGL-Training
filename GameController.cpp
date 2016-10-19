#include <GLFW/glfw3.h>
struct GameController {
private:


    double mousePos_x_old;
    double mousePos_x;
    double mousePos_y_old;
    double mousePos_y;

public:
    double MouseSpeed=1;

    bool PrepareAction;
    bool RequestAction;
    bool PrepareAction2;
    bool RequestAction2;

    bool GoLeft;
    bool GoRight;
    bool GoForward;
    bool GoBack;

    bool RequestJump;

    int MouseWheel;
    double Rotation_delta;
    double Pitch_delta;

    GLFWwindow* g_window;


    GameController(GLFWwindow* window) {
        clearInputContents();
        g_window=window; }

    void clearInputContents(void) {
        PrepareAction=false;
        RequestAction=false;
        PrepareAction2=false;
        RequestAction2=false;
        GoLeft=false;
        GoRight=false;
        GoForward=false;
        GoBack=false;
        RequestJump=false;
        Rotation_delta= 0;
        Pitch_delta=0;
        MouseWheel=0; }

    void readKeys(void) {
        glfwPollEvents ();

        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_W)) {
            GoForward=true; }

        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_S)) {
            GoBack=true; }

        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_D)) {
            GoRight=true; }

        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_A)) {
            GoLeft=true; }

        if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_SPACE)) {
            RequestJump=true; }

        mousePos_x_old=mousePos_x;
        mousePos_y_old=mousePos_y;
        glfwGetCursorPos(g_window,&mousePos_x,&mousePos_y);
        Rotation_delta=MouseSpeed*1.0f*((float)mousePos_x-(float)mousePos_x_old);
        Pitch_delta=MouseSpeed*1.0f*((float)mousePos_y-(float)mousePos_y_old); }

};
