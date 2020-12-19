#pragma once

#include "glad/glad.h"

class Quad {
    public:
        Quad();
        ~Quad();
        void draw();
    private:
        GLuint vao_;
        GLuint buffers_[2];
        GLuint diffuse_texture_; 
};