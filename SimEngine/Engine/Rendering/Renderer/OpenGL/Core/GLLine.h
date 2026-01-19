#pragma once

#include "Rendering/Core/Line.h"

class GLLine : public Line
{
public:
    GLLine();
    ~GLLine() override;
    
    void Draw() const override;
    
private:
    GLuint VAO{};
    GLuint VBO{};
};
