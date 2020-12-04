//
// Created by ttrata on 18.11.2020.
//

#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "rotation.h"
#include "glm/gtx/string_cast.hpp"
class Camera {
public:
    float logistic(float y) {
        return 1.0f/(1.0f+std::exp(-y));
    }

    float inverse_logistic(float x) {
        return std::log(x/(1.0f-x)); 
    }

    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        this->z_ = glm::normalize(eye - center);
        this->x_ = glm::normalize(glm::cross(up, this->z_));
        this->y_ = glm::normalize(glm::cross(this->z_, this->x_));

        this->position_ = eye;
        this->center_ = center;
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

    void rotate_around_point(float angle, const glm::vec3 &axis, const glm::vec3 &c) {
        auto R = rotation(angle, axis);
        this->x_ = R * this->x_;
        this->y_ = R * this->y_;
        this->z_ = R * this->z_;

        auto t = position_ - c;
        t = R * t;
        this->position_ = c + t;
    }

    void rotate_around_center(float angle, const glm::vec3 &axis) {
        rotate_around_point(angle, axis, center_); 
    }

    glm::mat4 view() const {
        glm::mat4 V(1.0f);
        for (int i = 0; i < 3; ++i) {
            V[i][0] = x_[i];
            V[i][1] = y_[i];
            V[i][2] = z_[i];
        }
        
        auto t = -glm::vec3{
                glm::dot(x_, position_),
                glm::dot(y_, position_),
                glm::dot(z_, position_),
        };
        V[3] = glm::vec4(t, 1.0f);
        
        return V;
    
    }

    glm::mat4 projection() const { return glm::perspective(this->fov_, this->aspect_, this->near_, this->far_); }

    glm::vec3 x() const { return this->x_; }
    glm::vec3 y() const { return this->y_; }
    glm::vec3 z() const { return this->z_; }
    glm::vec3 position() const { return this->position_; }
    glm::vec3 center() const { return this->center_; }

private:
    float fov_;
    float aspect_;
    float near_;
    float far_;
    glm::vec3 x_,y_,z_;
    glm::vec3 position_,center_;
};