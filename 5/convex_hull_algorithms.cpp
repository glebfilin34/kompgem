#include "convex_hull_algorithms.h"

int ConvexHullAlgorithms::orientation(const AlgorithmPoint& p, const AlgorithmPoint& q, const AlgorithmPoint& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

std::vector<AlgorithmPoint> ConvexHullAlgorithms::computeGrahamScan(const std::vector<AlgorithmPoint>& inputPoints) {
    if (inputPoints.size() < 3) {
        return {};
    }

    std::vector<AlgorithmPoint> points = inputPoints;

    int startIndex = 0;
    for (int i = 1; i < points.size(); i++) {
        if (points[i].y < points[startIndex].y ||
            (points[i].y == points[startIndex].y && points[i].x < points[startIndex].x)) {
            startIndex = i;
        }
    }

    std::vector<int> hullIndices;
    int current = startIndex;

    do {
        hullIndices.push_back(current);
        int next = (current + 1) % points.size();

        for (int i = 0; i < points.size(); i++) {
            if (orientation(points[current], points[i], points[next]) == 2) {
                next = i;
            }
        }

        current = next;
    } while (current != startIndex);

    std::vector<AlgorithmPoint> convexHull;
    for (int idx : hullIndices) {
        convexHull.push_back(points[idx]);
    }

    return convexHull;
}
