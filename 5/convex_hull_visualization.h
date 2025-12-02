#ifndef CONVEX_HULL_VISUALIZATION_H
#define CONVEX_HULL_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <vector>
#include "convex_hull_algorithms.h"

class VisualPoint {
public:
    QPointF pos;
    bool isDragging = false;
    VisualPoint(const QPointF& p) : pos(p) {}
};

class ConvexHullWidget : public QWidget {
    Q_OBJECT

public:
    ConvexHullWidget(QWidget *parent = nullptr);
    void clearPoints();
    void computeConvexHull();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::vector<VisualPoint> points;
    std::vector<QPointF> convexHull;
    bool onlineMode;

public slots:
    void setOnlineMode(bool enabled);
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    ConvexHullWidget *convexHullWidget;
};

#endif
