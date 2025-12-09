#ifndef SPHERE_ALGORITHMS_H
#define SPHERE_ALGORITHMS_H

#include <vector>
#include <cmath>

struct SpherePoint {
    float x, y, z;
    SpherePoint(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

struct SphereColor {
    float r, g, b;
    SphereColor(float r = 1, float g = 1, float b = 1) : r(r), g(g), b(b) {}
};

struct SphereVertex {
    SpherePoint position;
    SphereColor color;
};

class SphereAlgorithms {
public:
    static std::vector<SphereVertex> generateSphereVertices(float radius, int sectors, int stacks);
    static std::vector<SpherePoint> generateMeridians(float radius, int count, int segments);
    static std::vector<SpherePoint> generateParallels(float radius, int count, int segments);
    static SpherePoint rotatePoint(const SpherePoint& point, float angleX, float angleY, float angleZ);

private:
    static SphereColor getSphereColor(float latitude, float longitude);
};

#endif
