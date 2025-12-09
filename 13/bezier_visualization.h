#ifndef BEZIER_VISUALIZATION_H
#define BEZIER_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <vector>
#include "bezier_algorithms.h"

struct VisualPoint {
    QPointF pos;
    bool isDragging;
    VisualPoint(const QPointF& p) : pos(p), isDragging(false) {}
};

class BezierWidget : public QWidget {
    Q_OBJECT

public:
    BezierWidget(QWidget *parent = nullptr);
    void clearPoints();
    void updateCurves();
    void setOrder(int order);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::vector<VisualPoint> points;
    std::vector<QPointF> quadraticCurve;
    std::vector<QPointF> cubicCurve;
    std::vector<QPointF> nthOrderCurve;
    bool showQuadratic;
    bool showCubic;
    bool showNthOrder;
    int currentOrder;

public slots:
    void setShowQuadratic(bool show);
    void setShowCubic(bool show);
    void setShowNthOrder(bool show);
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    BezierWidget *bezierWidget;
};

#endif
