#include "sphere_algorithms.h"
#include <cmath>

SphereColor SphereAlgorithms::getSphereColor(float latitude, float longitude) {
    float latNorm = (latitude + M_PI_2) / M_PI;
    float lonNorm = longitude / (2 * M_PI);

    if (latNorm < 0.3f) {
        return SphereColor(0.2f, 0.5f, 0.8f);
    } else if (latNorm < 0.7f) {
        return SphereColor(0.8f, 0.3f, 0.2f);
    } else {
        return SphereColor(0.3f, 0.7f, 0.3f);
    }
}

std::vector<SphereVertex> SphereAlgorithms::generateSphereVertices(float radius, int sectors, int stacks) {
    std::vector<SphereVertex> vertices;

    for (int i = 0; i < stacks; ++i) {
        float lat0 = M_PI * (-0.5f + float(i) / stacks);
        float z0 = std::sin(lat0);
        float zr0 = std::cos(lat0);

        float lat1 = M_PI * (-0.5f + float(i + 1) / stacks);
        float z1 = std::sin(lat1);
        float zr1 = std::cos(lat1);

        for (int j = 0; j < sectors; ++j) {
            float lng0 = 2 * M_PI * float(j) / sectors;
            float x0 = std::cos(lng0);
            float y0 = std::sin(lng0);

            float lng1 = 2 * M_PI * float(j + 1) / sectors;
            float x1 = std::cos(lng1);
            float y1 = std::sin(lng1);

            SphereColor color1 = getSphereColor(lat0, lng0);
            SphereColor color2 = getSphereColor(lat1, lng1);

            vertices.push_back({SpherePoint(x0 * zr0 * radius, y0 * zr0 * radius, z0 * radius), color1});
            vertices.push_back({SpherePoint(x1 * zr0 * radius, y1 * zr0 * radius, z0 * radius), color1});
            vertices.push_back({SpherePoint(x0 * zr1 * radius, y0 * zr1 * radius, z1 * radius), color2});

            vertices.push_back({SpherePoint(x1 * zr0 * radius, y1 * zr0 * radius, z0 * radius), color1});
            vertices.push_back({SpherePoint(x1 * zr1 * radius, y1 * zr1 * radius, z1 * radius), color2});
            vertices.push_back({SpherePoint(x0 * zr1 * radius, y0 * zr1 * radius, z1 * radius), color2});
        }
    }

    return vertices;
}

std::vector<SpherePoint> SphereAlgorithms::generateMeridians(float radius, int count, int segments) {
    std::vector<SpherePoint> points;

    for (int m = 0; m < count; ++m) {
        float angle = 2 * M_PI * m / count;

        for (int i = 0; i <= segments; ++i) {
            float lat = M_PI * (-0.5f + float(i) / segments);
            float x = radius * std::cos(angle) * std::cos(lat);
            float y = radius * std::sin(angle) * std::cos(lat);
            float z = radius * std::sin(lat);
            points.push_back(SpherePoint(x, y, z));
        }
    }

    return points;
}

std::vector<SpherePoint> SphereAlgorithms::generateParallels(float radius, int count, int segments) {
    std::vector<SpherePoint> points;

    for (int p = 1; p < count; ++p) {
        float lat = M_PI * (-0.5f + float(p) / count);
        float z = radius * std::sin(lat);
        float r = radius * std::cos(lat);

        for (int i = 0; i <= segments; ++i) {
            float lng = 2 * M_PI * i / segments;
            float x = r * std::cos(lng);
            float y = r * std::sin(lng);
            points.push_back(SpherePoint(x, y, z));
        }
    }

    return points;
}

SpherePoint SphereAlgorithms::rotatePoint(const SpherePoint& point, float angleX, float angleY, float angleZ) {
    float radX = angleX * M_PI / 180.0f;
    float radY = angleY * M_PI / 180.0f;
    float radZ = angleZ * M_PI / 180.0f;

    float x = point.x;
    float y = point.y;
    float z = point.z;

    float tempY = y;
    y = y * std::cos(radX) - z * std::sin(radX);
    z = tempY * std::sin(radX) + z * std::cos(radX);

    float tempX = x;
    x = x * std::cos(radY) + z * std::sin(radY);
    z = -tempX * std::sin(radY) + z * std::cos(radY);

    tempX = x;
    x = x * std::cos(radZ) - y * std::sin(radZ);
    y = tempX * std::sin(radZ) + y * std::cos(radZ);

    return SpherePoint(x, y, z);
}
