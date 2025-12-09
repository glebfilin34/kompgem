#include "polygon_bool_algorithms.h"
#include <algorithm>
#include <set>
#include <queue>
#include <stack>

bool PolygonBoolean::pointInPolygon(const BoolPoint& p, const PolygonContour& contour) {
    if (contour.points.size() < 3) return false;

    bool inside = false;
    for (size_t i = 0, j = contour.points.size() - 1; i < contour.points.size(); j = i++) {
        if (((contour.points[i].y > p.y) != (contour.points[j].y > p.y)) &&
            (p.x < (contour.points[j].x - contour.points[i].x) * (p.y - contour.points[i].y) /
                           (contour.points[j].y - contour.points[i].y) + contour.points[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

bool PolygonBoolean::pointInPolygonList(const BoolPoint& p, const std::vector<PolygonContour>& polygons) {
    for (const auto& contour : polygons) {
        if (pointInPolygon(p, contour)) {
            return !contour.isHole;
        }
    }
    return false;
}

bool PolygonBoolean::segmentsIntersect(const BoolPoint& a1, const BoolPoint& a2,
                                       const BoolPoint& b1, const BoolPoint& b2,
                                       BoolPoint& intersect) {
    BoolPoint d1 = a2 - a1;
    BoolPoint d2 = b2 - b1;

    double cross = d1.cross(d2);
    if (std::abs(cross) < 1e-12) return false;

    double t = (b1 - a1).cross(d2) / cross;
    double u = (b1 - a1).cross(d1) / cross;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        intersect.x = a1.x + t * d1.x;
        intersect.y = a1.y + t * d1.y;
        return true;
    }
    return false;
}

double PolygonBoolean::polygonArea(const PolygonContour& contour) {
    if (contour.points.size() < 3) return 0;

    double area = 0;
    for (size_t i = 0; i < contour.points.size(); i++) {
        size_t j = (i + 1) % contour.points.size();
        area += contour.points[i].x * contour.points[j].y;
        area -= contour.points[j].x * contour.points[i].y;
    }
    return area / 2.0;
}

void PolygonBoolean::ensureWindingOrder(PolygonContour& contour, bool clockwise) {
    if (contour.points.size() < 3) return;

    double area = polygonArea(contour);
    bool isClockwise = area < 0;

    if (isClockwise != clockwise) {
        std::reverse(contour.points.begin(), contour.points.end());
    }
}

std::vector<PolygonContour> PolygonBoolean::booleanOperation(
    const std::vector<PolygonContour>& poly1,
    const std::vector<PolygonContour>& poly2,
    Operation op) {

    std::vector<PolygonContour> result;

    if (poly1.empty() && poly2.empty()) return result;

    if (op == UNION) {
        result = poly1;
        result.insert(result.end(), poly2.begin(), poly2.end());

        for (auto& contour : result) {
            ensureWindingOrder(contour, false);
            contour.isHole = false;
        }
        return result;
    }

    if (op == INTERSECTION) {
        if (poly1.empty() || poly2.empty()) return result;

        for (const auto& contour1 : poly1) {
            if (contour1.points.size() < 3) continue;

            for (const auto& contour2 : poly2) {
                if (contour2.points.size() < 3) continue;

                std::vector<BoolPoint> allPoints;
                std::set<BoolPoint> uniquePoints;

                for (const auto& p : contour1.points) {
                    if (pointInPolygon(p, contour2)) {
                        if (uniquePoints.find(p) == uniquePoints.end()) {
                            allPoints.push_back(p);
                            uniquePoints.insert(p);
                        }
                    }
                }

                for (const auto& p : contour2.points) {
                    if (pointInPolygon(p, contour1)) {
                        if (uniquePoints.find(p) == uniquePoints.end()) {
                            allPoints.push_back(p);
                            uniquePoints.insert(p);
                        }
                    }
                }

                for (size_t i = 0; i < contour1.points.size(); i++) {
                    size_t next_i = (i + 1) % contour1.points.size();
                    const BoolPoint& a1 = contour1.points[i];
                    const BoolPoint& a2 = contour1.points[next_i];

                    for (size_t j = 0; j < contour2.points.size(); j++) {
                        size_t next_j = (j + 1) % contour2.points.size();
                        const BoolPoint& b1 = contour2.points[j];
                        const BoolPoint& b2 = contour2.points[next_j];

                        BoolPoint intersect;
                        if (segmentsIntersect(a1, a2, b1, b2, intersect)) {
                            if (uniquePoints.find(intersect) == uniquePoints.end()) {
                                allPoints.push_back(intersect);
                                uniquePoints.insert(intersect);
                            }
                        }
                    }
                }

                if (allPoints.size() >= 3) {
                    if (allPoints.size() > 3) {
                        BoolPoint center(0, 0);
                        for (const auto& p : allPoints) {
                            center.x += p.x;
                            center.y += p.y;
                        }
                        center.x /= allPoints.size();
                        center.y /= allPoints.size();

                        std::sort(allPoints.begin(), allPoints.end(),
                                  [center](const BoolPoint& a, const BoolPoint& b) {
                                      double angleA = std::atan2(a.y - center.y, a.x - center.x);
                                      double angleB = std::atan2(b.y - center.y, b.x - center.x);
                                      return angleA < angleB;
                                  });
                    }

                    PolygonContour newContour;
                    newContour.points = allPoints;
                    newContour.isHole = false;
                    ensureWindingOrder(newContour, false);
                    result.push_back(newContour);
                }
            }
        }
        return result;
    }

    if (op == DIFFERENCE) {
        if (poly1.empty()) return result;
        if (poly2.empty()) return poly1;

        for (const auto& contour1 : poly1) {
            if (contour1.points.size() < 3) continue;

            std::vector<BoolPoint> allPoints;
            std::set<BoolPoint> uniquePoints;

            for (const auto& p : contour1.points) {
                bool insideAny = false;
                for (const auto& contour2 : poly2) {
                    if (pointInPolygon(p, contour2)) {
                        insideAny = true;
                        break;
                    }
                }
                if (!insideAny && uniquePoints.find(p) == uniquePoints.end()) {
                    allPoints.push_back(p);
                    uniquePoints.insert(p);
                }
            }

            for (const auto& contour2 : poly2) {
                if (contour2.points.size() < 3) continue;

                for (size_t i = 0; i < contour1.points.size(); i++) {
                    size_t next_i = (i + 1) % contour1.points.size();
                    const BoolPoint& a1 = contour1.points[i];
                    const BoolPoint& a2 = contour1.points[next_i];

                    for (size_t j = 0; j < contour2.points.size(); j++) {
                        size_t next_j = (j + 1) % contour2.points.size();
                        const BoolPoint& b1 = contour2.points[j];
                        const BoolPoint& b2 = contour2.points[next_j];

                        BoolPoint intersect;
                        if (segmentsIntersect(a1, a2, b1, b2, intersect)) {
                            if (uniquePoints.find(intersect) == uniquePoints.end()) {
                                allPoints.push_back(intersect);
                                uniquePoints.insert(intersect);
                            }
                        }
                    }
                }
            }

            if (allPoints.size() >= 3) {
                if (allPoints.size() > 3) {
                    BoolPoint center(0, 0);
                    for (const auto& p : allPoints) {
                        center.x += p.x;
                        center.y += p.y;
                    }
                    center.x /= allPoints.size();
                    center.y /= allPoints.size();

                    std::sort(allPoints.begin(), allPoints.end(),
                              [center](const BoolPoint& a, const BoolPoint& b) {
                                  double angleA = std::atan2(a.y - center.y, a.x - center.x);
                                  double angleB = std::atan2(b.y - center.y, b.x - center.x);
                                  return angleA < angleB;
                              });
                }

                PolygonContour newContour;
                newContour.points = allPoints;
                newContour.isHole = false;
                ensureWindingOrder(newContour, false);
                result.push_back(newContour);
            }
        }
        return result;
    }

    return result;
}
