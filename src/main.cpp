#include <sstream>
#include <istream>
#include <string>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>

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
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void charCallback(GLFWwindow* window, unsigned int codepoint);

// pointer objects
GLFWwindow * window = nullptr;
nanogui::Screen * screen = nullptr;
nanogui::ColorWheel * cWheel = nullptr;
nanogui::ColorWheel * lWheel = nullptr;
nanogui::Slider * aSlider = nullptr;
nanogui::Slider * xSlider = nullptr;
nanogui::Slider * ySlider = nullptr;
nanogui::Slider * zSlider = nullptr;

Scene * scene = nullptr;
Geometry * geometry = nullptr;
Geometry * geometry2 = nullptr;
unsigned int currentObj = 0;

// global variables
std::string windowTitle = "Procedural";

int main() {

    srand(time(0));

    // magic
    glewExperimental = GL_TRUE;

    // Init GLEW and GLFW
    if(initializeOpenGL() == -1)
        return -1;

  

    // Create scene here.
    scene = new Scene();
    

    geometry = new Geometry();
    geometry->loadObject("bunny");
    //geometry->loadObject("cube");
    geometry->translate(glm::vec3(.5f, -.5f, 0.0f));

    scene->addGeometry(geometry);
    //scene->addGeometry(geometry2);
    scene->initialize();
    
      //init nanogui
    if(!initGUI() )
        return false;

    //set mouse handlers
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, mouseMotion);
    glfwSetScrollCallback(window, mouseScroll);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, charCallback);
    
    // render-loop
    do {
        calculateFPS(1.0, windowTitle);
           
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        scene->getGeometryAt(currentObj)->setColor(glm::vec4(cWheel->color().r(), cWheel->color().g(), cWheel->color().b(), aSlider->value()));
        scene->setLightColor(glm::vec3(lWheel->color().r(), lWheel->color().g(), lWheel->color().b()));
        
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

    nanogui::PopupButton * popupBtn;
    nanogui::PopupButton * objPopupBtn;
    nanogui::Popup * popup;
    nanogui::Popup * objPopup;
    nanogui::Button * colorBtn;
    nanogui::Button * objBtn;
    nanogui::Button * phongBtn;
    nanogui::Button * frostBtn;

    screen = new nanogui::Screen();
    screen->initialize(window, true);   
    
    // Geometry window
    nanogui::Window * gWindow = new nanogui::Window(screen, "Geometry settings");
    gWindow->setPosition(Eigen::Vector2i(15, 40));

    nanogui::GridLayout * layout = new nanogui::GridLayout(nanogui::Orientation::Horizontal, 2, nanogui::Alignment::Middle, 15, 5);
    layout->setColAlignment( { nanogui::Alignment::Maximum, nanogui::Alignment::Fill });
    layout->setSpacing(0, 10);
    gWindow->setLayout(layout);

    new nanogui::Label(gWindow, "object", "sans-bold");
    objPopupBtn = new nanogui::PopupButton(gWindow, "Choose Obj");
    objPopupBtn->setFixedSize(Eigen::Vector2i(100, 25));
    objPopup = objPopupBtn->popup();
    objPopup->setLayout(new nanogui::GroupLayout());

    new nanogui::Label(gWindow, "color", "sans-bold");
    
    popupBtn = new nanogui::PopupButton(gWindow, "", 0);
    popupBtn->setBackgroundColor(nanogui::Color(255, 0, 0, 255));
    popupBtn->setFontSize(16);
    popupBtn->setFixedSize(Eigen::Vector2i(100, 20));
    popup = popupBtn->popup();
    popup->setLayout(new nanogui::GroupLayout());

    cWheel = new nanogui::ColorWheel(popup);

    colorBtn = new nanogui::Button(popup, "Choose color");
    colorBtn->setFixedSize(Eigen::Vector2i(100, 25));

    colorBtn->setChangeCallback([colorBtn, popupBtn](bool pushed) {
        if(pushed) {
            popupBtn->setBackgroundColor(cWheel->color());
            popupBtn->setPushed(false);
        }
    });

    new nanogui::Label(gWindow, "Alpha", "sans");
    aSlider = new nanogui::Slider(gWindow);
    aSlider->setValue(1.0f);

    phongBtn = new nanogui::Button(gWindow, "Phong");
    phongBtn->setFlags(nanogui::Button::RadioButton);
    phongBtn->setFixedSize(Eigen::Vector2i(100, 20));
    phongBtn->setChangeCallback([currentObj](bool pushed) {
        if(pushed) {
            std::cout << "phong" << std::endl;
            scene->getGeometryAt(currentObj)->setPhongActive();
            std::cout << scene->getGeometryAt(currentObj)->phongActive() << std::endl;
        }
    });
    phongBtn->setPushed(true);

    frostBtn = new nanogui::Button(gWindow, "Frost");
    frostBtn->setFlags(nanogui::Button::RadioButton);
    frostBtn->setFixedSize(Eigen::Vector2i(100, 20));
    frostBtn->setChangeCallback([currentObj](bool pushed) {
        if(pushed) {
            std::cout << "frost" << std::endl;
            scene->getGeometryAt(currentObj)->setFrostActive();
            std::cout << scene->getGeometryAt(currentObj)->phongActive() << std::endl;
        }
    });
    //frostBtn->setPushed(true);

    //SET CALLBACKS AFTER CHWHEEL AND SLIDER HAS BEEN INITED! FULT MEN MAN MÅSTE VÄNTA!
    for(unsigned int i = 0; i < scene->getNumGeometries(); i++) {
        objBtn = new nanogui::Button(objPopup, "Object" + std::to_string(i+1));
        objBtn->setFixedSize(Eigen::Vector2i(100, 25));

        objBtn->setChangeCallback([objBtn, objPopupBtn, i, popupBtn, phongBtn, frostBtn](bool pushed) {
            if(pushed) {
                currentObj = i;
                glm::vec3 color = scene->getGeometryAt(currentObj)->getColor();
                cWheel->setColor(nanogui::Color(color[0], color[1], color[2], 1.0));
                popupBtn->setBackgroundColor(cWheel->color());
                aSlider->setValue(color[3]);
                objPopupBtn->setPushed(false);

                if(scene->getGeometryAt(currentObj)->phongActive()) {
                    phongBtn->setPushed(true);
                    frostBtn->setPushed(false);
                }
                else {
                    phongBtn->setPushed(false);
                    frostBtn->setPushed(true);
                }
            }
        });
    }

    // Light window
    nanogui::Window * lWindow = new nanogui::Window(screen, "Light settings");
    lWindow->setPosition(Eigen::Vector2i(15, 200));
    lWindow->setLayout(layout);

    new nanogui::Label(lWindow, "color", "sans-bold");
    popupBtn = new nanogui::PopupButton(lWindow, "", 0);
    popupBtn->setBackgroundColor(nanogui::Color(255, 255, 255, 255));
    popupBtn->setFontSize(16);
    popupBtn->setFixedSize(Eigen::Vector2i(100, 20));
    popup = popupBtn->popup();
    popup->setLayout(new nanogui::GroupLayout());

    lWheel = new nanogui::ColorWheel(popup);
    lWheel->setColor(nanogui::Color(255, 255, 255, 255));

    colorBtn = new nanogui::Button(popup, "Choose color");
    colorBtn->setFixedSize(Eigen::Vector2i(100, 25));

    colorBtn->setChangeCallback([colorBtn, popupBtn](bool pushed) {
        if(pushed)
            popupBtn->setBackgroundColor(lWheel->color());
            popupBtn->setPushed(false);
    });

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

    //check if we are interacting with a nano gui object
    if(!screen->mouseButtonCallbackEvent(button, action, mods)) {

        //if not treat the click as a click for the trackball camera
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
}

// handles mouse movement, send updated positions to the scene
void mouseMotion(GLFWwindow* window, double x, double y) {
    if(!screen->cursorPosCallbackEvent(x, y))
        scene->updateCameraPosition(x, y);
}

void mouseScroll(GLFWwindow* window, double x, double y) {
    if(!screen->scrollCallbackEvent(x, y))
        scene->updateCameraZoom(x, y);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    screen->keyCallbackEvent(key, scancode, action, mods);
}

void charCallback(GLFWwindow* window, unsigned int codepoint) {
    screen->charCallbackEvent(codepoint);
}