#include "bezier_algorithms.h"

double BezierAlgorithms::binomialCoefficient(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;

    double result = 1;
    for (int i = 1; i <= k; i++) {
        result = result * (n - k + i) / i;
    }
    return result;
}

AlgoPoint BezierAlgorithms::bezierPoint(const std::vector<AlgoPoint>& points, double t) {
    int n = points.size() - 1;
    AlgoPoint result(0, 0);

    for (int i = 0; i <= n; i++) {
        double coeff = binomialCoefficient(n, i) * pow(1 - t, n - i) * pow(t, i);
        result.x += coeff * points[i].x;
        result.y += coeff * points[i].y;
    }

    return result;
}

std::vector<AlgoPoint> BezierAlgorithms::computeBezierQuadratic(const std::vector<AlgoPoint>& points, int segments) {
    if (points.size() < 3) return {};

    std::vector<AlgoPoint> curve;
    for (int i = 0; i <= segments; i++) {
        double t = static_cast<double>(i) / segments;
        AlgoPoint p = bezierPoint(points, t);
        curve.push_back(p);
    }

    return curve;
}

std::vector<AlgoPoint> BezierAlgorithms::computeBezierCubic(const std::vector<AlgoPoint>& points, int segments) {
    if (points.size() < 4) return {};

    std::vector<AlgoPoint> curve;
    for (int i = 0; i <= segments; i++) {
        double t = static_cast<double>(i) / segments;
        AlgoPoint p = bezierPoint(points, t);
        curve.push_back(p);
    }

    return curve;
}

std::vector<AlgoPoint> BezierAlgorithms::computeBezierNthOrder(const std::vector<AlgoPoint>& points, int segments, int order) {
    if (points.size() < order + 1) return {};

    std::vector<AlgoPoint> curve;
    for (int i = 0; i <= segments; i++) {
        double t = static_cast<double>(i) / segments;
        AlgoPoint p = bezierPoint(points, t);
        curve.push_back(p);
    }

    return curve;
}
