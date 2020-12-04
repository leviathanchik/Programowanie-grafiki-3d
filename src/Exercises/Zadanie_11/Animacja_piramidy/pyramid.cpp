//
// Created by ttrata on 18.11.2020.
//

#include "pyramid.h"
#include <vector>

Pyramid::Pyramid() {
 /*
     Tu dodajemy kod tworzący bufory indeksów i wierzchołków oraz obiekt VAO,
     Potem ładujemy dane wierzchołków i indeksow do buforów  i  podłaczamy je do atrubutów w szaderze
*/
    std::vector<GLfloat> vertices = {
           0.5f, 0.5f, 1.0f, 0.7f, 0.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.7f, 0.0f, 0.0f,
           0.5f, 0.5f, 1.0f, 0.7f, 0.2f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.7f, 0.2f, 0.0f,
           1.0f, 1.0f, 0.0f, 0.7f, 0.2f, 0.0f,
           0.5f, 0.5f, 1.0f, 0.2f, 0.7f, 0.0f,
           1.0f, 1.0f, 0.0f, 0.2f, 0.7f, 0.0f,
           1.0f, 0.0f, 0.0f, 0.2f, 0.7f, 0.0f,
           0.5f, 0.5f, 1.0f, 0.0f, 0.2f, 0.7f,
           1.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.7f,
           0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.7f,
           0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    };

    glGenBuffers(2, this->buffers_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::vector<GLushort> indices = {
           0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,14,15,12 // wypisujemy tyle elementów ile mamy wierzchołków
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), 
                        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(sizeof(GLfloat)*3));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
}  

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(2, this->buffers_);
} 
 
void Pyramid::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    glBindVertexArray(0);
}  
