#include <iostream>
#include <string>
#include <nanogui/nanogui.h>
#include <GLFW/glfw3.h>

enum test_enum {
    Item1 = 0,
    Item2,
    Item3
};

bool bvar = true;
int ivar = 12345678;
double dvar = 3.1415926;
float fvar = (float)dvar;
std::string strval = "A string";
test_enum enumval = Item2;
nanogui::Color colval(0.5f, 0.5f, 0.7f, 1.f);

GLFWwindow *window = nullptr;
nanogui::Screen *screen = nullptr;
nanogui::ref<nanogui::Window> nanoguiWindow;

int screenWidth;
int screenHeight;

using namespace nanogui;

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
    glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    

    // Create a nanogui screen and pass the glfw pointer to initialize
    screen = new nanogui::Screen();
    screen->initialize(window, true);


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
    glfwSwapBuffers(window);

}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << nanoguiWindow.get() << std::endl;
    std::cout << "New framebuffer size: " << width << " x " << height << std::endl;
    screenWidth = width;
    screenHeight = height;
    nanoguiWindow->setPosition(Eigen::Vector2i(screenWidth - nanoguiWindow->width(), 0));
}


int main(int argc, char **argv) {
    createGLContexts();

    bool enabled = true;

    nanogui::FormHelper *gui = new nanogui::FormHelper(screen);
    nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Form helper example");

   
    gui->addGroup("Basic types");
    gui->addVariable("bool", bvar)->setTooltip("Test tooltip.");
    gui->addVariable("string", strval);

    gui->addGroup("Validating fields");
    gui->addVariable("int", ivar)->setSpinnable(true);
    gui->addVariable("float", fvar)->setTooltip("Test.");
    gui->addVariable("double", dvar)->setSpinnable(true);
    

    gui->addGroup("Complex types");
    gui->addVariable("Enumeration", enumval, enabled)->setItems({ "Item 1", "Item 2", "Item 3" });
    gui->addVariable("Color", colval);

    gui->addGroup("Other widgets");
    gui->addButton("A button", []() { std::cout << "Button pressed." << std::endl; })->setTooltip("Testing a much longer tooltip, that will wrap around to new lines multiple times.");;



    // Create a scroll panel and add it to the main window
    auto scrollWindow = new nanogui::Window(screen, "Scroll Panel");
    scrollWindow->setPosition(Eigen::Vector2i(scrollWindow->width(), 0));
    // scrollWindow->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, 10, 5));
    scrollWindow->setSize(Eigen::Vector2i(300, screen->height()));

    /*Host for the desired layout*/
    // auto dataViewList = new Widget(scrollWindow);
    // dataViewList->setLayout(new BoxLayout(Orientation::Vertical,Alignment::Fill));
    
    // auto dummy_button = new Button(dataViewList, "Dummy Button");

    ref<VScrollPanel> scrollpanel = new VScrollPanel(scrollWindow);
    scrollpanel->setFixedHeight(scrollWindow->height());
    scrollpanel->setFixedWidth(scrollWindow->width());
    scrollpanel->setPosition(Eigen::Vector2i(0, 30));

    auto scrollContainer = new Widget(scrollpanel);
    scrollContainer->setFixedSize(Vector2i(scrollWindow->width(), 1000));

    auto table = new GridLayout(/*dir*/Orientation::Horizontal, /*dir size*/4,
                              Alignment::Fill, /*margin*/3,/*spacing*/6);
    scrollpanel->setLayout(table);

    int buttonHeight = 30; // Height of each button
    int buttonSpacing = 5; // Spacing between buttons
    int yPos = buttonSpacing; // Initial y-position
    /* Children of VScrollPanel, to be arranged and scrolled*/
    for (int i=0;  i< 16; i++) {
        auto button = new nanogui::Button(scrollContainer, "Button " + std::to_string(i));
        button->setFixedSize(Eigen::Vector2i(180, buttonHeight));  // Set the size of each button
        button->setPosition(Eigen::Vector2i(0, yPos)); // Set the position of the button
        yPos += buttonHeight + buttonSpacing; // Increment y-position for the next button
        std::cout << "Button: " << std::to_string(i) << std::endl;
    }
    screen->setVisible(true);
    screen->performLayout();


    nanoguiWindow -> setPosition(Eigen::Vector2i(screenWidth - nanoguiWindow->width(), 0));
    nanoguiWindow->setSize(Eigen::Vector2i(nanoguiWindow->width(), screen->height()));

    

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

    glfwSetWindowSizeCallback(window, windowSizeCallback);

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
