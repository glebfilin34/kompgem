#ifndef POINT_IN_HULL_VISUALIZATION_H
#define POINT_IN_HULL_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <vector>
#include "point_in_hull_algorithms.h"

class PolygonWidget : public QWidget {
    Q_OBJECT

public:
    PolygonWidget(QWidget *parent = nullptr);
    void clearAll();
    void buildConvexHull();
    void checkPoint();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    std::vector<QPointF> points;
    std::vector<AlgoPoint2D> convexHullPoints;
    QPolygonF convexHull;
    QPointF testPoint;
    bool onlineMode;
    bool testPointDragging;
    bool hullBuilt;
    int result;

public slots:
    void setOnlineMode(bool enabled);
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    PolygonWidget *polygonWidget;
};

#endif
