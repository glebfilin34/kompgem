#ifndef DELAUNAY_ALGORITHMS_H
#define DELAUNAY_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

struct AlgoPoint {
    double x, y;
    AlgoPoint() : x(0), y(0) {}
    AlgoPoint(double x, double y) : x(x), y(y) {}
};

struct AlgoTriangle {
    int p1, p2, p3;
    AlgoTriangle(int a, int b, int c) : p1(a), p2(b), p3(c) {}
    bool operator==(const AlgoTriangle& other) const {
        return p1 == other.p1 && p2 == other.p2 && p3 == other.p3;
    }
};

struct AlgoEdge {
    int p1, p2;
    AlgoEdge(int a, int b) : p1(std::min(a, b)), p2(std::max(a, b)) {}
    bool operator==(const AlgoEdge& other) const {
        return p1 == other.p1 && p2 == other.p2;
    }
    bool operator<(const AlgoEdge& other) const {
        if (p1 != other.p1) return p1 < other.p1;
        return p2 < other.p2;
    }
};

class DelaunayAlgorithms {
public:
    static std::vector<AlgoTriangle> computeDelaunay(const std::vector<AlgoPoint>& points);

private:
    static bool isPointInCircumcircle(const AlgoPoint& a, const AlgoPoint& b, const AlgoPoint& c, const AlgoPoint& p);
};

#endif
