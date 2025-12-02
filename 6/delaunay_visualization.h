#ifndef DELAUNAY_VISUALIZATION_H
#define DELAUNAY_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <vector>
#include "delaunay_algorithms.h"

struct VisualPoint {
    QPointF pos;
    bool isDragging = false;
    VisualPoint(const QPointF& p) : pos(p) {}
};

struct VisualTriangle {
    int p1, p2, p3;
    VisualTriangle(int a, int b, int c) : p1(a), p2(b), p3(c) {}
};

class DelaunayWidget : public QWidget {
    Q_OBJECT

public:
    DelaunayWidget(QWidget *parent = nullptr);
    void clearPoints();
    void computeDelaunay();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::vector<VisualPoint> points;
    std::vector<VisualTriangle> triangles;
    bool onlineMode;

public slots:
    void setOnlineMode(bool enabled);
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    DelaunayWidget *delaunayWidget;
};

#endif
