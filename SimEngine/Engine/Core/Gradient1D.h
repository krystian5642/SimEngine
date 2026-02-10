#pragma once

class Texture;

struct Gradient1DPoint
{
    float position;
    glm::vec3 color;
};

class Gradient1D
{
public:
    void AddPoint(float position, const glm::vec3& color);
    
    glm::vec3 GetColor(float time) const;
    TexturePtr GetTexture() const;
    
    void ClearPoints() { points.clear(); }
    
private:
    std::vector<Gradient1DPoint> points;
};
