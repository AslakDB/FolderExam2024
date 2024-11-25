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
}

std::vector<glm::vec3> TrackingPath::CalculateBSpline(const std::vector<glm::vec3>& Points, int numPoints)
{
    std::vector<glm::vec3> splinePoints;
    int n = Points.size() - 1; // Number of control points
    int k = 3; // Cubic B-spline

    // Ensure there are enough control points
    if (Points.size() < k + 1) {
        throw std::runtime_error("Not enough control points for cubic B-spline");
    }

    for (int i = 0; i <= numPoints - 1; ++i) {
        float t = static_cast<float>(i) / (numPoints - 1) * (n - k + 1);
        glm::vec3 point = Points[0]; // Initialize with the first control point

        for (int j = 0; j <= n; ++j) {
            float basis = B(j, k, t);
            point += basis * (Points[j] - Points[0]); // Offset by the first control point
        }

        // Add the point to the spline points
        splinePoints.push_back(point);
    }

    return splinePoints;
}

void TrackingPath::DrawPoints(model& pathModel, const std::vector<glm::vec3>& Points)
{
    pathModel.vertices.clear(); // Clear existing vertices

    for (const auto& element : Points) {
        pathModel.vertices.emplace_back(element, glm::vec3(0.f), glm::vec3(0.6f), 0);
    }

    pathModel.Bind();
}

void TrackingPath::CreateBSplinePath(model& pathModel,const std::vector<glm::vec3>& Points)
{
    try {
        // Generate 100 points for the B-spline
        std::vector<glm::vec3> splinePoints = CalculateBSpline(Points, 1000);
        // Clear existing vertices
        pathModel.vertices.clear();

        // Add the generated spline points to the path model
        for (const auto& spline_point : splinePoints) {
            pathModel.vertices.emplace_back(spline_point, glm::vec3(0.f), glm::vec3(0.6f), 0);
        }

        // Set the model to be drawn as a line
        pathModel.isLine = true;

        // Bind the model to update the GPU buffers
        pathModel.Bind();
    } catch (const std::exception& e) {
        std::cerr << "Exception in CreateBSplinePath: " << e.what() << std::endl;
    }
}
