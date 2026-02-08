#include "Gradient1D.h"

#include "Rendering/Renderer/Renderer.h"

void Gradient1D::AddPoint(float position, const glm::vec3& color)
{
    points.push_back({position, color});
}

glm::vec3 Gradient1D::GetColor(float time) const
{
    if (points.empty())
    {
        return glm::vec3(0.0f);
    }

    if (time <= points.front().position)
    {
        return points.front().color;
    }
    
    if (time >= points.back().position)
    {
        return points.back().color;
    }
    
    for (int i = 0; i < points.size() - 1; i++)
    {
        if (time >= points[i].position && time <= points[i + 1].position)
        {
            auto& first = points[i];
            auto& second = points[i + 1];
            
            return glm::mix(first.color, second.color, (time - first.position) / (second.position - first.position));
        }
    }
    return glm::vec3(1.0f);
}

std::shared_ptr<Texture> Gradient1D::GetTexture() const
{
    unsigned int resolution = 256;
    std::vector<unsigned char> colors(resolution * 3);
    
    for (int i = 0; i < resolution; i++)
    {
        const auto time = static_cast<float>(i) / static_cast<float>(resolution - 1);
        const auto color = GetColor(time);
        
        colors[3*i] = static_cast<unsigned char>(color.x * 255.0f);
        colors[3*i + 1] = static_cast<unsigned char>(color.y * 255.0f);
        colors[3*i + 2] = static_cast<unsigned char>(color.z * 255.0f);
    }
    
    return Renderer::CreateTextureStatic(colors.data(), resolution, 1);
}
