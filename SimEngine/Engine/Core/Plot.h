#pragma once

class Plot
{
public:
    
    void AddPoint(float x, float y);
    void ClearPoints();
    int GetPointCount() const { return static_cast<int>(xValues.size()); }
    void SetMaxPoints(int newMaxPoints);
    
    const float* GetXValuesData() const { return xValues.data(); }
    const float* GetYValuesData() const { return yValues.data(); }
    
private:
    std::vector<float> xValues;
    std::vector<float> yValues;
    
    int maxPoints{-1};
    size_t index{0};
};

struct RuntimePlotData
{
    Plot plot;
    float addPointInterval{1.0f / 60.0f};
        
    float elapsedTime{0.0f};
    float timeSinceLastAddPoint{0.0f};
};
