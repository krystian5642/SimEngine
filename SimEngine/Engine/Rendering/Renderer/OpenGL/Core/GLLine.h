#pragma once

#include "Rendering/Core/Line.h"

namespace SimEngine
{
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
}
