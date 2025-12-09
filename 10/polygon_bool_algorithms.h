#ifndef POLYGON_BOOL_ALGORITHMS_H
#define POLYGON_BOOL_ALGORITHMS_H

#include <vector>
#include <cmath>

struct BoolPoint {
    double x, y;
    BoolPoint(double x = 0, double y = 0) : x(x), y(y) {}
    
    bool operator==(const BoolPoint& other) const {
        return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
    }
    
    bool operator!=(const BoolPoint& other) const {
        return !(*this == other);
    }
    
    bool operator<(const BoolPoint& other) const {
        if (std::abs(x - other.x) > 1e-9) return x < other.x;
        return y < other.y;
    }
    
    BoolPoint operator-(const BoolPoint& other) const {
        return BoolPoint(x - other.x, y - other.y);
    }
    
    double cross(const BoolPoint& other) const {
        return x * other.y - y * other.x;
    }
    
    double dot(const BoolPoint& other) const {
        return x * other.x + y * other.y;
    }
};

struct PolygonContour {
    std::vector<BoolPoint> points;
    bool isHole;
    
    PolygonContour() : isHole(false) {}
};

class PolygonBoolean {
public:
    enum Operation { INTERSECTION, UNION, DIFFERENCE };
    
    static std::vector<PolygonContour> booleanOperation(
        const std::vector<PolygonContour>& poly1,
        const std::vector<PolygonContour>& poly2,
        Operation op);
    
private:
    static bool pointInPolygon(const BoolPoint& p, const PolygonContour& contour);
    static bool pointInPolygonList(const BoolPoint& p, const std::vector<PolygonContour>& polygons);
    static bool segmentsIntersect(const BoolPoint& a1, const BoolPoint& a2,
                                  const BoolPoint& b1, const BoolPoint& b2,
                                  BoolPoint& intersect);
    static double polygonArea(const PolygonContour& contour);
    static void ensureWindingOrder(PolygonContour& contour, bool clockwise);
};

#endif