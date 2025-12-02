#ifndef POINT_IN_HULL_ALGORITHMS_H
#define POINT_IN_HULL_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

struct AlgoPoint2D {
    double x, y;
    AlgoPoint2D(double x = 0, double y = 0) : x(x), y(y) {}
};

class PointInPolygonAlgorithms {
private:
    static double cross(const AlgoPoint2D& a, const AlgoPoint2D& b, const AlgoPoint2D& c);
    static double distance(const AlgoPoint2D& a, const AlgoPoint2D& b);
    static double pointToSegmentDistance(const AlgoPoint2D& p, const AlgoPoint2D& a, const AlgoPoint2D& b);

public:
    static int check(const AlgoPoint2D& point, const std::vector<AlgoPoint2D>& polygon);
};

class ConvexHullAlgorithms {
public:
    static std::vector<AlgoPoint2D> compute(const std::vector<AlgoPoint2D>& points);
};

#endif
