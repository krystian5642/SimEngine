#include "Plot.h"

void Plot::AddPoint(float x, float y)
{
    if (maxPoints > 0 && xValues.size() == maxPoints)
    {
        xValues.erase(xValues.begin());
        yValues.erase(yValues.begin());
    }
    xValues.push_back(x);
    yValues.push_back(y);
}

void Plot::ClearPoints()
{
    xValues.clear();
    yValues.clear();
}

void Plot::SetMaxPoints(int newMaxPoints)
{
    ClearPoints();
    
    maxPoints = newMaxPoints;
    
    xValues.reserve(maxPoints);
    yValues.reserve(maxPoints);
}
