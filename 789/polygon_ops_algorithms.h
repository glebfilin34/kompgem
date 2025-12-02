#ifndef POLYGON_OPS_ALGORITHMS_H
#define POLYGON_OPS_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>

struct AlgoPoint {
    double x, y;
    AlgoPoint() : x(0), y(0) {}
    AlgoPoint(double x, double y) : x(x), y(y) {}

    AlgoPoint operator+(const AlgoPoint& other) const { return AlgoPoint(x + other.x, y + other.y); }
    AlgoPoint operator-(const AlgoPoint& other) const { return AlgoPoint(x - other.x, y - other.y); }
    AlgoPoint operator*(double scalar) const { return AlgoPoint(x * scalar, y * scalar); }
    double dot(const AlgoPoint& other) const { return x * other.x + y * other.y; }
    double cross(const AlgoPoint& other) const { return x * other.y - y * other.x; }
    double dist2() const { return x*x + y*y; }
};

class AlgoPolygon {
public:
    std::vector<AlgoPoint> points;

    void addPoint(const AlgoPoint& p) { points.push_back(p); }
    void clear() { points.clear(); }
    bool empty() const { return points.empty(); }
    size_t size() const { return points.size(); }

    void computeConvexHull();
};

class PolygonOperationsAlgorithms {
public:
    enum Operation { INTERSECTION, UNION, DIFFERENCE };

    static AlgoPolygon computeOperation(const AlgoPolygon& poly1, const AlgoPolygon& poly2, Operation op);

private:
    static AlgoPolygon computeIntersection(const AlgoPolygon& poly1, const AlgoPolygon& poly2);
    static AlgoPolygon computeUnion(const AlgoPolygon& poly1, const AlgoPolygon& poly2);
    static AlgoPolygon computeDifference(const AlgoPolygon& poly1, const AlgoPolygon& poly2);
    static bool isPointInsidePolygon(const AlgoPoint& p, const std::vector<AlgoPoint>& polygon);
    static bool lineSegmentIntersection(const AlgoPoint& a1, const AlgoPoint& a2,
                                        const AlgoPoint& b1, const AlgoPoint& b2, AlgoPoint& result);
};

#endif
