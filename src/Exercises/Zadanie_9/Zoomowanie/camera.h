#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
    float logistic(float y) {
        return 1.0f/(1.0f+std::exp(-y));
    }

    float inverse_logistic(float x) {
        return std::log(x/(1.0f-x)); 
    }

    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        this->V_ = glm::lookAt(eye, center, up);
    }

    void perspective(float fov, float aspect, float near, float far) {
        this->fov_ = fov;
        this->aspect_ = aspect;
        this->near_ = near;
        this->far_ = far;
    }

    void set_aspect(float aspect) {
        this->aspect_ = aspect;
    }

    void zoom(float y_offset) {
        auto x = this->fov_/glm::pi<float>();
        auto y = inverse_logistic(x);
        y+=y_offset;
        x = logistic(y); 
        this->fov_ = x*glm::pi<float>(); 
    }

    glm::mat4 view() const { return this->V_; }

    glm::mat4 projection() const { return glm::perspective(this->fov_, this->aspect_, this->near_, this->far_); }

private:
    float fov_;
    float aspect_;
    float near_;
    float far_;

    glm::mat4 V_;
};