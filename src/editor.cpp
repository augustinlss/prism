#include <nanogui/nanogui.h>

#include "editor.h"

using namespace nanogui;

Window *developer_ = nullptr; 

void createLeftSidebar(Screen* screen, Window* scrollWindow) {
    // Create a scroll panel and add it to the main windowd
    scrollWindow->setSize(Eigen::Vector2i(300, screen->height()));
    scrollWindow->setPosition(Eigen::Vector2i(0, 0));

    /*Host for the desired layout*/
    // auto dataViewList = new Widget(scrollWindow);
    // dataViewList->setLayout(new BoxLayout(Orientation::Vertical,Alignment::Fill));
    
    // auto dummy_button = new Button(dataViewList, "Dummy Button");

    ref<VScrollPanel> scrollpanel = new VScrollPanel(scrollWindow);
    scrollpanel->setFixedHeight(scrollWindow->height() - 30);
    scrollpanel->setFixedWidth(scrollWindow->width());
    scrollpanel->setPosition(Eigen::Vector2i(0, 30));

    auto scrollContainer = new Widget(scrollpanel);
    scrollContainer->setFixedSize(Vector2i(scrollWindow->width(), 1200));

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
}

void windowSizeCallback(GLFWwindow *window, int width, int height) {
    std::cout << developer_->getRefCount() << std::endl;
    std::cout << "New framebuffer size: " << width << " x " << height << std::endl;
    // screenWidth = width;
    // screenHeight = height;
    developer_->setPosition(Eigen::Vector2i(width - developer_->width(), 0));
}

void createRightSidebar(Screen* screen, Window* scrollWindow, GLFWwindow* window, int screenWidth) {
    developer_ = scrollWindow;

    // Create a scroll panel and add it to the main window
    scrollWindow->setSize(Eigen::Vector2i(300, screen->height()));
    scrollWindow->setPosition(Eigen::Vector2i(screen->width() - scrollWindow->width(), 0));

    glfwSetWindowSizeCallback(window, windowSizeCallback);
    /*Host for the desired layout*/
    // auto dataViewList = new Widget(scrollWindow);
    // dataViewList->setLayout(new BoxLayout(Orientation::Vertical,Alignment::Fill));
    
    // auto dummy_button = new Button(dataViewList, "Dummy Button");

    ref<VScrollPanel> scrollpanel = new VScrollPanel(developer_);
    scrollpanel->setFixedHeight(developer_->height() - 30);
    scrollpanel->setFixedWidth(developer_->width());
    scrollpanel->setPosition(Eigen::Vector2i(0, 30));

    auto scrollContainer = new Widget(scrollpanel);
    scrollContainer->setFixedSize(Vector2i(developer_->width(), 1200));

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

    auto slider = new ColorSlider(scrollContainer);
    slider->setFixedSize(Eigen::Vector2i(180, buttonHeight));
    slider->setPosition(Eigen::Vector2i(0, yPos));

}



