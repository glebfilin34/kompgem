#ifndef BEZIER_ALGORITHMS_H
#define BEZIER_ALGORITHMS_H

#include <vector>
#include <cmath>

struct AlgoPoint {
    double x, y;
    AlgoPoint(double x = 0, double y = 0) : x(x), y(y) {}
};

class BezierAlgorithms {
public:
    static std::vector<AlgoPoint> computeBezierQuadratic(const std::vector<AlgoPoint>& points, int segments);
    static std::vector<AlgoPoint> computeBezierCubic(const std::vector<AlgoPoint>& points, int segments);
    static std::vector<AlgoPoint> computeBezierNthOrder(const std::vector<AlgoPoint>& points, int segments, int order);

private:
    static AlgoPoint bezierPoint(const std::vector<AlgoPoint>& points, double t);
    static double binomialCoefficient(int n, int k);
};

#endif
