#ifndef CONVEX_HULL_ALGORITHMS_H
#define CONVEX_HULL_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <cmath>

struct AlgorithmPoint {
    double x, y;
    AlgorithmPoint() : x(0), y(0) {}
    AlgorithmPoint(double x, double y) : x(x), y(y) {}
};

class ConvexHullAlgorithms {
public:
    static std::vector<AlgorithmPoint> computeGrahamScan(const std::vector<AlgorithmPoint>& points);

private:
    static int orientation(const AlgorithmPoint& p, const AlgorithmPoint& q, const AlgorithmPoint& r);
};

#endif
