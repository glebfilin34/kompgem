#ifndef SPHERE_VISUALIZATION_H
#define SPHERE_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <vector>
#include "sphere_algorithms.h"

class SphereWidget : public QWidget {
    Q_OBJECT

public:
    SphereWidget(QWidget *parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawSphere(QPainter &painter);
    
    std::vector<SphereVertex> sphereVertices;
    std::vector<SpherePoint> meridians;
    std::vector<SpherePoint> parallels;
    
    float rotationX;
    float rotationY;
    float rotationZ;
    
    QPoint lastMousePos;
    bool mousePressed;
    
    QTimer animationTimer;
    bool autoRotate;
    int centerX, centerY;
    float scale;

private slots:
    void animate();
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    SphereWidget *sphereWidget;
};

#endif
