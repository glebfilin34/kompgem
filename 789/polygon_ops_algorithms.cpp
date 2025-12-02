#include "polygon_ops_algorithms.h"

void AlgoPolygon::computeConvexHull() {
    if (points.size() < 3) return;

    int n = points.size();
    int minIdx = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[minIdx].y ||
            (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
            minIdx = i;
        }
    }
    std::swap(points[0], points[minIdx]);

    AlgoPoint pivot = points[0];
    std::sort(points.begin() + 1, points.end(), [pivot](const AlgoPoint& a, const AlgoPoint& b) {
        AlgoPoint vecA = a - pivot;
        AlgoPoint vecB = b - pivot;
        double cross = vecA.cross(vecB);
        if (std::abs(cross) > 1e-9) return cross > 0;
        return vecA.dist2() < vecB.dist2();
    });

    std::vector<AlgoPoint> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    for (int i = 2; i < n; i++) {
        while (hull.size() >= 2) {
            AlgoPoint& p1 = hull[hull.size() - 2];
            AlgoPoint& p2 = hull[hull.size() - 1];
            AlgoPoint& p3 = points[i];

            if ((p2 - p1).cross(p3 - p1) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(points[i]);
    }

    points = hull;
}

AlgoPolygon PolygonOperationsAlgorithms::computeOperation(const AlgoPolygon& poly1, const AlgoPolygon& poly2, Operation op) {
    switch (op) {
    case INTERSECTION:
        return computeIntersection(poly1, poly2);
    case UNION:
        return computeUnion(poly1, poly2);
    case DIFFERENCE:
        return computeDifference(poly1, poly2);
    }
    return AlgoPolygon();
}

AlgoPolygon PolygonOperationsAlgorithms::computeIntersection(const AlgoPolygon& poly1, const AlgoPolygon& poly2) {
    AlgoPolygon result;
    std::vector<AlgoPoint> points1 = poly1.points;
    std::vector<AlgoPoint> points2 = poly2.points;

    for (const auto& p : points1) {
        if (isPointInsidePolygon(p, points2)) {
            result.addPoint(p);
        }
    }

    for (const auto& p : points2) {
        if (isPointInsidePolygon(p, points1)) {
            result.addPoint(p);
        }
    }

    for (size_t i = 0; i < points1.size(); i++) {
        size_t next_i = (i + 1) % points1.size();
        for (size_t j = 0; j < points2.size(); j++) {
            size_t next_j = (j + 1) % points2.size();

            AlgoPoint intersect;
            if (lineSegmentIntersection(points1[i], points1[next_i],
                                        points2[j], points2[next_j], intersect)) {
                result.addPoint(intersect);
            }
        }
    }

    if (!result.empty()) {
        result.computeConvexHull();
    }
    return result;
}

AlgoPolygon PolygonOperationsAlgorithms::computeUnion(const AlgoPolygon& poly1, const AlgoPolygon& poly2) {
    AlgoPolygon result;
    std::vector<AlgoPoint> allPoints;
    allPoints.insert(allPoints.end(), poly1.points.begin(), poly1.points.end());
    allPoints.insert(allPoints.end(), poly2.points.begin(), poly2.points.end());

    for (const auto& p : allPoints) {
        result.addPoint(p);
    }

    if (!result.empty()) {
        result.computeConvexHull();
    }
    return result;
}

AlgoPolygon PolygonOperationsAlgorithms::computeDifference(const AlgoPolygon& poly1, const AlgoPolygon& poly2) {
    AlgoPolygon result;
    std::vector<AlgoPoint> points1 = poly1.points;
    std::vector<AlgoPoint> points2 = poly2.points;

    for (const auto& p : points1) {
        if (!isPointInsidePolygon(p, points2)) {
            result.addPoint(p);
        }
    }

    for (size_t i = 0; i < points1.size(); i++) {
        size_t next_i = (i + 1) % points1.size();
        for (size_t j = 0; j < points2.size(); j++) {
            size_t next_j = (j + 1) % points2.size();

            AlgoPoint intersect;
            if (lineSegmentIntersection(points1[i], points1[next_i],
                                        points2[j], points2[next_j], intersect)) {
                result.addPoint(intersect);
            }
        }
    }

    if (!result.empty()) {
        result.computeConvexHull();
    }
    return result;
}

bool PolygonOperationsAlgorithms::isPointInsidePolygon(const AlgoPoint& p, const std::vector<AlgoPoint>& polygon) {
    if (polygon.size() < 3) return false;

    bool inside = false;
    for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) /
                           (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

bool PolygonOperationsAlgorithms::lineSegmentIntersection(const AlgoPoint& a1, const AlgoPoint& a2,
                                                          const AlgoPoint& b1, const AlgoPoint& b2, AlgoPoint& result) {
    AlgoPoint d1 = a2 - a1;
    AlgoPoint d2 = b2 - b1;

    double cross = d1.cross(d2);
    if (std::abs(cross) < 1e-9) return false;

    double t = (b1 - a1).cross(d2) / cross;
    double u = (b1 - a1).cross(d1) / cross;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        result = a1 + d1 * t;
        return true;
    }
    return false;
}
