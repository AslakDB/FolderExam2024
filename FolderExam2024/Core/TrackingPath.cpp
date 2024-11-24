#include "TrackingPath.h"

void TrackingPath::AddPoint(glm::vec3& point)
{

    points.push_back(point);
}

float TrackingPath::B(int i, int k, float t)
{
    
    if (k == 0)
    {
        return (i <= t && t < i + 1) ? 1.0f : 0.0f;
    }
    else
    {
        float w1 = (t - i) / k;
        float w2 = (i + k + 1 - t) / k;
        return w1 * B(i, k - 1, t) + w2 * B(i + 1, k - 1, t);
    }
    return 1.f;
}


std::vector<glm::vec3> TrackingPath::CalculateBSpline(std::vector<glm::vec3> Points, int numPoints)
{
    
    std::vector<glm::vec3> splinePoints;
    int n = Points.size() - 1;
    int k = 3; // Cubic B-spline

    // Ensure there are enough control points
    if (Points.size() < k + 1) {
        throw std::runtime_error("Not enough control points for cubic B-spline");
    }

    for (int i = 0; i <= numPoints - 1; ++i) {
        float t = static_cast<float>(i) / numPoints;
        glm::vec3 point(0.0f);

        for (int j = 0; j <= n; ++j) {
            float basis = B(j, k, t);
            point += basis * Points[j];
        }

        splinePoints.push_back(point);
    }

    return splinePoints;
   
}

void TrackingPath::DrawPoints(model& pathModel, std::vector<glm::vec3> Points)
{
    
    pathModel.vertices.clear(); // Clear existing vertices

    for (const auto& element : Points) {
        pathModel.vertices.emplace_back(element, glm::vec3(0.f), glm::vec3(0.6f), 0);
    }

    pathModel.Bind();
}

void TrackingPath::CreateBSplinePath(model& pathModel, std::vector<glm::vec3> Points)
{
    try {
        std::vector<glm::vec3> splinePoints = CalculateBSpline(Points, 100); // Generate 100 points
        pathModel.isLine = true;
        DrawPoints(pathModel, splinePoints);
    } catch (const std::exception& e) {
        std::cerr << "Exception in CreateBSplinePath: " << e.what() << std::endl;
    }
}

