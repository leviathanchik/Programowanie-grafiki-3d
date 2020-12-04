//
// Created by ttrata on 18.11.2020.
//

#pragma once

#include "glad/glad.h"

class Pyramid {
    public:
        Pyramid();
        ~Pyramid();
        void draw();
    private:
        GLuint vao_;
        GLuint buffers_[2];
};