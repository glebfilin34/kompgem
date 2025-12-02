#include "delaunay_algorithms.h"

bool DelaunayAlgorithms::isPointInCircumcircle(const AlgoPoint& a, const AlgoPoint& b, const AlgoPoint& c, const AlgoPoint& p) {
    double d = 2 * (a.x * (b.y - c.y) +
                    b.x * (c.y - a.y) +
                    c.x * (a.y - b.y));

    double ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
                 (b.x * b.x + b.y * b.y) * (c.y - a.y) +
                 (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;

    double uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
                 (b.x * b.x + b.y * b.y) * (a.x - c.x) +
                 (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;

    AlgoPoint center(ux, uy);
    double radius = std::sqrt((a.x - center.x) * (a.x - center.x) +
                              (a.y - center.y) * (a.y - center.y));

    double distance = std::sqrt((p.x - center.x) * (p.x - center.x) +
                                (p.y - center.y) * (p.y - center.y));

    return distance <= radius;
}

std::vector<AlgoTriangle> DelaunayAlgorithms::computeDelaunay(const std::vector<AlgoPoint>& inputPoints) {
    if (inputPoints.size() < 3) {
        return {};
    }

    std::vector<AlgoPoint> points = inputPoints;

    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;

    for (const auto& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    double dx = maxX - minX;
    double dy = maxY - minY;
    double deltaMax = std::max(dx, dy);
    double midX = (minX + maxX) / 2.0;
    double midY = (minY + maxY) / 2.0;

    int p1 = points.size();
    int p2 = p1 + 1;
    int p3 = p1 + 2;

    std::vector<AlgoTriangle> triangleList;
    triangleList.emplace_back(p1, p2, p3);

    std::vector<AlgoPoint> tempPoints = points;
    tempPoints.emplace_back(midX - 20 * deltaMax, midY - deltaMax);
    tempPoints.emplace_back(midX, midY + 20 * deltaMax);
    tempPoints.emplace_back(midX + 20 * deltaMax, midY - deltaMax);

    for (int i = 0; i < points.size(); i++) {
        std::vector<AlgoEdge> polygon;
        std::vector<AlgoTriangle> toRemove;

        for (const auto& triangle : triangleList) {
            if (isPointInCircumcircle(tempPoints[triangle.p1],
                                      tempPoints[triangle.p2],
                                      tempPoints[triangle.p3],
                                      points[i])) {
                toRemove.push_back(triangle);

                polygon.emplace_back(triangle.p1, triangle.p2);
                polygon.emplace_back(triangle.p2, triangle.p3);
                polygon.emplace_back(triangle.p3, triangle.p1);
            }
        }

        for (const auto& triangle : toRemove) {
            triangleList.erase(
                std::remove(triangleList.begin(), triangleList.end(), triangle),
                triangleList.end()
                );
        }

        std::vector<AlgoEdge> uniqueEdges;
        for (const auto& edge : polygon) {
            int count = std::count(polygon.begin(), polygon.end(), edge);
            if (count == 1) {
                uniqueEdges.push_back(edge);
            }
        }

        for (const auto& edge : uniqueEdges) {
            triangleList.emplace_back(edge.p1, edge.p2, i);
        }
    }

    std::vector<AlgoTriangle> result;
    for (const auto& triangle : triangleList) {
        if (triangle.p1 < points.size() && triangle.p2 < points.size() && triangle.p3 < points.size()) {
            result.push_back(triangle);
        }
    }

    return result;
}
