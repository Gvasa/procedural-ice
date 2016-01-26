#include <sstream>
#include <istream>
#include <math.h>



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <nanogui/nanogui.h>

#include "../include/Scene.h"
#include "../include/Geometry.h"

// functions
int initializeOpenGL();
bool initGUI();
double calculateFPS(double, std::string);
void mouseButton(GLFWwindow* window, int button, int andction, int mods);
void mouseMotion(GLFWwindow* window, double x, double y);
void mouseScroll(GLFWwindow* window, double x, double y);

// pointer objects
GLFWwindow * window = nullptr;
nanogui::Screen * screen = nullptr;

Scene * scene = nullptr;
Geometry * geometry = nullptr;

// global variables
std::string windowTitle = "Procedural";

// constants


bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";


int main() {

    // magic
    glewExperimental = GL_TRUE;

    // Init GLEW and GLFW
    if(initializeOpenGL() == -1)
        return -1;

    //init nanogui
    if(!initGUI() )
        return false;

    // Create scene here.
    scene = new Scene();
    

    geometry = new Geometry();
    geometry->loadObject("cube");
    //geometry->translate(glm::vec3(0.5f, -1.0f, 0.0f));
   
    scene->addGeometry(geometry);
    scene->initialize();


    //set mouse handlers
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, mouseMotion);
    glfwSetScrollCallback(window, mouseScroll);

    // render-loop
    do {
        calculateFPS(1.0, windowTitle);
           
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene
        scene->render();

        screen->drawWidgets();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );


    //deallocate nanogui -> call for all ~()
    nanogui::shutdown();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return EXIT_SUCCESS;
}


int initializeOpenGL() {
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow( WIDTH, HEIGHT, windowTitle.c_str(), NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. You might be having an old GPU\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    std::cout << "Running OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    return 0;
}

bool initGUI() {
    nanogui::init();

    screen = new nanogui::Screen();
    screen->initialize(window, true);   
    
    nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
    gui->addWindow(Eigen::Vector2i(15, 15), "formhelper");

    gui->addGroup("Basic types");
    gui->addVariable("bool", bvar);
    gui->addVariable("string", strval);

    gui->addGroup("Validating fields");
    gui->addVariable("int", ivar);
    gui->addVariable("float", fvar);
    gui->addVariable("double", dvar);

    gui->addGroup("Other widgets");
    gui->addButton("A button", [](){ std::cout << "Button pressed." << std::endl; });

    screen->setVisible(true);
    screen->performLayout();

    return true;
}

double calculateFPS(double timeInterval = 1.0, std::string windowTitle = "NONE") {

    // Static values which only get initialised the first time the function runs
    static double startTime  =  glfwGetTime(); // Set the initial time to now
    static double fps        =  0.0;           // Set the initial FPS value to 0.0
 
    // Set the initial frame count to -1.0 (it gets set to 0.0 on the next line). Because
    // we don't have a start time we simply cannot get an accurate FPS value on our very
    // first read if the time interval is zero, so we'll settle for an FPS value of zero instead.
    static double frameCount =  -1.0;
 
    frameCount++;
 
    // Ensure the time interval between FPS checks is sane (low cap = 0.0 i.e. every frame, high cap = 10.0s)
    if (timeInterval < 0.0)
    {
        timeInterval = 0.0;
    }
    else if (timeInterval > 10.0)
    {
        timeInterval = 10.0;
    }
 
    // Get the duration in seconds since the last FPS reporting interval elapsed
    // as the current time minus the interval start time
    double duration = glfwGetTime() - startTime;
 
    // If the time interval has elapsed...
    if (duration > timeInterval)
    {
        // Calculate the FPS as the number of frames divided by the duration in seconds
        fps = round(frameCount / duration);
 
        // If the user specified a window title to append the FPS value to...
        if (windowTitle != "NONE")
        {
            // Convert the fps value into a string using an output stringstream
            std::ostringstream stream;
            stream << fps;
            std::string fpsString = stream.str();
 
            // Append the FPS value to the window title details
            windowTitle += " | FPS: " + fpsString;
 
            // Convert the new window title to a c_str and set it
            const char* pszConstString = windowTitle.c_str();
            glfwSetWindowTitle(window, pszConstString);
        }
        else // If the user didn't specify a window to append the FPS to then output the FPS to the console
        {
            std::cout << "FPS: " << fps << std::endl;
        }
 
        // Reset the frame count to zero and set the initial time to be now
        frameCount  = 0.0;
        startTime   = glfwGetTime();
    }
 
    // Return the current FPS - doesn't have to be used if you don't want it!
    return fps;
}

// Function that handles input from mouse, sends the position to scene 
void mouseButton(GLFWwindow* window, int button, int action, int mods) {
    if(button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    screen->mouseButtonCallbackEvent(button, action, mods);

    switch(action) {
        case GLFW_PRESS:
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            scene->mouseButtonClick(x, y);
            break;
        case GLFW_RELEASE:
            scene->mouseButtonRelease();
            break;
        default:
            break;
    }
}

// handles mouse movement, send updated positions to the scene
void mouseMotion(GLFWwindow* window, double x, double y) {
    if(!screen->cursorPosCallbackEvent(x, y))
        scene->updateCameraPosition(x, y);
}

void mouseScroll(GLFWwindow* window, double x, double y) {
    scene->updateCameraZoom(x, y);
}