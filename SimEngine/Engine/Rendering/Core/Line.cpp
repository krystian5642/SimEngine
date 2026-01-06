#include "Line.h"

namespace SimEngine
{
    Line::Line()
    {
    }

    Line::~Line()
    {
    }

    void Line::SetPoints(const std::vector<glm::vec3>& newPoints)
    {
        if (maxLength > 0.0f && newPoints.size() > 1)
        {
            points.clear();
            points.reserve(newPoints.size());
            
            points.push_back(newPoints[0]);
        
            currentLength = 0.0f;
            for (size_t i = 1; i < newPoints.size(); i++)
            {
                currentLength += glm::distance(newPoints[i - 1], newPoints[i]);
                if (currentLength < maxLength)
                {
                    points.push_back(newPoints[i]);
                }
            }
        }
        else
        {
            currentLength = 0.0f;
            points = newPoints;
            
            if (!points.empty())
            {
                for (size_t i = 1; i < newPoints.size(); i++)
                {
                    currentLength += glm::distance(newPoints[i - 1], newPoints[i]);
                }
            }
        }
    }

    void Line::AddPoint(const glm::vec3& point)
    {
        if (maxLength > 0.0f)
        {
            if (!points.empty())
            {
                const auto& last = points.back();
                const float lastSegmentLength = glm::distance(last, point);
                while (points.size() > 1 && currentLength + lastSegmentLength > maxLength)
                {
                    const auto& first = points[0];
                    const auto& second = points[1];
                
                    const float firstSegmentLength = glm::distance(first, second);
                    currentLength -= firstSegmentLength;
                
                    points.erase(points.begin());
                }
                
                currentLength += lastSegmentLength;
                points.push_back(point);
            }
            else
            {
                currentLength = 0.0f;
                points.push_back(point);
            }
        }
        else
        {
            if (!points.empty())
            {
                currentLength += glm::distance(points.back(), point);
            }
            points.push_back(point);
        }
    }
}
