//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application/utils.h"

void SimpleShapeApplication::init() {

    this->set_camera(new Camera);
    this->set_controler(new CameraControler(this->camera()));
    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    this->pyramid = new Pyramid;

    auto u_pvm_index = glGetUniformBlockIndex(program, "Transformations"); 
    if (u_pvm_index == GL_INVALID_INDEX) { 
        std::cout << "Cannot find PVM uniform block in program" << std::endl; 
    } else { 
        glUniformBlockBinding(program, u_pvm_index, 1); 
    }

    int w, h;
    std::tie(w, h) = frame_buffer_size();

    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/3.0;
    float near_ = 0.5f;
    float far_ = 100.0f;
    this->camera()->perspective(fov_, aspect_, near_, far_); 
    this->camera()->look_at(glm::vec3{0.0, 0.0, 30.0}, glm::vec3{0.5f, 0.5f, 0.0f}, glm::vec3{0.0, 0.0, 1.0});
    
    glGenBuffers(1, &this->u_pvm_buffer_);
    
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &this->camera()->view()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->u_pvm_buffer_);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    start_ = std::chrono::steady_clock::now();

    rotation_period = 4.0f;
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    auto aspect = (float) w / h;
    this->camera()->set_aspect(aspect);
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
        Application::scroll_callback(xoffset, yoffset);
        this->camera()->zoom(yoffset / 30.0f);
    }

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (this->controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            this->controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            this->controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (this->controler_) {
        this->controler_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::frame() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    float rotation_angle = glm::two_pi<float>()*elapsed_time/rotation_period;

    glm::mat4 main_TR = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

    glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation_angle ,glm::vec3(0.0f, 0.0f, 1.0f)); // niech raz kompilator wymyśli typ.
    glBindBuffer(GL_UNIFORM_BUFFER, this->u_pvm_buffer_);

    auto rotated = this->camera()->view() * R;
    
    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/orbital_rotation_period;
    auto a = 10.0f;
    auto b = 8.0f;
    float x = a*cos(orbital_rotation_angle);
    float y = b*sin(orbital_rotation_angle); 

    glm::mat4 O = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,0.0f));
    auto orbited = this->camera()->view() * O * R;

    auto moon_rotation_period = 10.0f;
    auto moon_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/moon_rotation_period;
    float x_m = 3.0f*cos(moon_rotation_angle);
    float y_m = 3.0f*sin(moon_rotation_angle);
    glm::mat4 O_moon = glm::translate(glm::mat4(1.0f), glm::vec3(x_m,y_m,0));
    glm::mat4 R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.794f, 0.794f, 0.794f)); // 0.794 ~= 1/sqrt3(2)

    glm::mat4 moon_transform = this->camera()->view() * O_moon * R_moon * S;

    auto satellite_rotation_period = 2.0f;
    auto satellite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satellite_rotation_period;
    float x_sat = 1.5*cos(satellite_rotation_angle);
    float z_sat = 1.5*sin(satellite_rotation_angle);
    glm::mat4 O_sat = glm::translate(glm::mat4(1.0f), glm::vec3(x_sat, 0, z_sat));
    glm::mat4 R_sat = glm::rotate(glm::mat4(1.0f), satellite_rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S_sat = glm::scale(glm::mat4(1.0f), glm::vec3(0.63f, 0.63f, 0.63f)); // 0.63 ~= 1/sqrt3(4)
    glm::mat4 sat_transform = this->camera()->view() * O_sat * R_sat * S_sat;

    glm::mat4 transforms[] = {
        rotated, 
        orbited, 
        moon_transform,
        sat_transform};

    for (glm::mat4 transform : transforms) {
        auto moved_to_main_pyramid_center = transform * main_TR;

        glBindBuffer(GL_UNIFORM_BUFFER, this->u_pvm_buffer_);

        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->camera()->projection()[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &moved_to_main_pyramid_center[0]);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        this->pyramid->draw();
    }

}
