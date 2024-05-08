#include <iostream>
#include <string>
#include <nanogui/nanogui.h>
#include <GLFW/glfw3.h>

#include "editor.h"

using namespace nanogui;

GLFWwindow *window = nullptr;
nanogui::Screen *screen = nullptr;
nanogui::ref<nanogui::Window> nanoguiWindow;

Window *navigator = nullptr;
Window *developer = nullptr;    
Window *menubar = nullptr;

int screenWidth;
int screenHeight;



void createGLContexts() {
    if(!glfwInit()) {
        return;
    }

    glfwSetTime(0);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


    // Get screen resolution
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screenWidth = mode -> width;
    screenHeight = mode -> height;

    //Create glfw window object
    window = glfwCreateWindow(screenWidth, screenHeight, "Prism", nullptr, nullptr);
    if(window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // Check if glad is initialized
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Could not initialize GLAD!");
    }
    glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    

    // Create a nanogui screen and pass the glfw pointer to initialize
    screen = new nanogui::Screen();
    screen->initialize(window, true);

    menubar = new Window(screen, "");
    menubar->setSize(Eigen::Vector2i(screen->width(), 18));
    navigator = new Window(screen, "Navigator");
    developer = new Window(screen, "Develop");  


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glfwSwapBuffers(window);

    createLeftSidebar(screen, navigator);
    createRightSidebar(screen, developer, window, screenWidth);   

}


int main(int argc, char **argv) {
    createGLContexts();
    
    bool enabled = true;

    // Create a popup button
    PopupButton *popupButton = new PopupButton(menubar, "Popup Button");

    // Set the size of the popup button
    int buttonWidth = 150; // Adjust the width as needed
    popupButton->setFixedSize(Vector2i(buttonWidth, menubar->height()));

    // Set the position of the popup button
    int xPos = 10; // Adjust the x-position as needed
    popupButton->setPosition(Vector2i(xPos, 0));

    // Create a popup window
    Popup *popup = popupButton->popup();

    // Add widgets to the popup window (e.g., buttons, labels, etc.)
    Button *popupContentButton = new Button(popup, "Popup Content");
    popupContentButton->setFixedSize(Vector2i(120, 30)); // Adjust size as needed

    // Set a callback function for the popup content button
    popupContentButton->setCallback([]() {
        // Handle button click for popup content
        std::cout << "Popup content button clicked" << std::endl;
    });


    
    screen->setVisible(true);
    screen->performLayout();

    
    glfwSetCursorPosCallback(window,
            [](GLFWwindow *, double x, double y) {
            screen->cursorPosCallbackEvent(x, y);
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow *, int button, int action, int modifiers) {
            screen->mouseButtonCallbackEvent(button, action, modifiers);
        }
    );

    glfwSetKeyCallback(window,
        [](GLFWwindow *, int key, int scancode, int action, int mods) {
            screen->keyCallbackEvent(key, scancode, action, mods);
        }
    );

    glfwSetCharCallback(window,
        [](GLFWwindow *, unsigned int codepoint) {
            screen->charCallbackEvent(codepoint);
        }
    );

    glfwSetDropCallback(window,
        [](GLFWwindow *, int count, const char **filenames) {
            screen->dropCallbackEvent(count, filenames);
        }
    );

    glfwSetScrollCallback(window,
        [](GLFWwindow *, double x, double y) {
            screen->scrollCallbackEvent(x, y);
       }
    );

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow *, int width, int height) {
            screen->resizeCallbackEvent(width, height);
        }
    );

    

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw nanogui
        screen->drawContents();
        screen->drawWidgets();

        glfwSwapBuffers(window);
    }

    // Terminate Nanogui
    delete screen;
    nanogui::shutdown();

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
