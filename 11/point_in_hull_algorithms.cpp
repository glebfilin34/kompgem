#include "point_in_hull_algorithms.h"

double PointInPolygonAlgorithms::cross(const AlgoPoint2D& a, const AlgoPoint2D& b, const AlgoPoint2D& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

double PointInPolygonAlgorithms::distance(const AlgoPoint2D& a, const AlgoPoint2D& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double PointInPolygonAlgorithms::pointToSegmentDistance(const AlgoPoint2D& p, const AlgoPoint2D& a, const AlgoPoint2D& b) {
    double l2 = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    if (l2 == 0) return distance(p, a);

    double t = std::max(0.0, std::min(1.0, ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / l2));
    AlgoPoint2D projection(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
    return distance(p, projection);
}

int PointInPolygonAlgorithms::check(const AlgoPoint2D& point, const std::vector<AlgoPoint2D>& polygon) {
    if (polygon.size() < 3) return 0;

    double minDist = std::numeric_limits<double>::max();
    for (int i = 0; i < polygon.size(); i++) {
        int j = (i + 1) % polygon.size();
        double dist = pointToSegmentDistance(point, polygon[i], polygon[j]);
        if (dist < minDist) minDist = dist;
    }

    double delta = 3.0;
    if (minDist < delta) return 2;

    bool inside = false;
    for (int i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
        if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
            (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }

    return inside ? 1 : 0;
}

std::vector<AlgoPoint2D> ConvexHullAlgorithms::compute(const std::vector<AlgoPoint2D>& points) {
    if (points.size() < 3) return points;

    std::vector<AlgoPoint2D> sortedPoints = points;
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const AlgoPoint2D& a, const AlgoPoint2D& b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });

    std::sort(sortedPoints.begin() + 1, sortedPoints.end(), [&](const AlgoPoint2D& a, const AlgoPoint2D& b) {
        double angleA = std::atan2(a.y - sortedPoints[0].y, a.x - sortedPoints[0].x);
        double angleB = std::atan2(b.y - sortedPoints[0].y, b.x - sortedPoints[0].x);
        return angleA < angleB || (angleA == angleB &&
                                   std::hypot(a.x - sortedPoints[0].x, a.y - sortedPoints[0].y) <
                                       std::hypot(b.x - sortedPoints[0].x, b.y - sortedPoints[0].y));
    });

    std::vector<AlgoPoint2D> hull;
    hull.push_back(sortedPoints[0]);
    hull.push_back(sortedPoints[1]);

    for (int i = 2; i < sortedPoints.size(); i++) {
        while (hull.size() >= 2) {
            AlgoPoint2D& a = hull[hull.size() - 2];
            AlgoPoint2D& b = hull[hull.size() - 1];
            AlgoPoint2D& c = sortedPoints[i];

            if ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0) break;
            hull.pop_back();
        }
        hull.push_back(sortedPoints[i]);
    }

    return hull;
}
