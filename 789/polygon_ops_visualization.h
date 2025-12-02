#ifndef POLYGON_OPS_VISUALIZATION_H
#define POLYGON_OPS_VISUALIZATION_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMouseEvent>
#include <QPainter>
#include <vector>
#include "polygon_ops_algorithms.h"

struct VisualPoint {
    QPointF pos;
    bool isDragging = false;
    VisualPoint(const QPointF& p) : pos(p) {}
};

class VisualPolygon {
public:
    std::vector<VisualPoint> points;

    void addPoint(const VisualPoint& p) { points.push_back(p); }
    void clear() { points.clear(); }
    bool empty() const { return points.empty(); }
    size_t size() const { return points.size(); }
};

class PolygonCanvas : public QWidget {
    Q_OBJECT

public:
    enum Mode { FIRST_POLYGON, SECOND_POLYGON, RESULT };

    PolygonCanvas(QWidget *parent = nullptr);
    void setOperation(PolygonOperationsAlgorithms::Operation op);

public slots:
    void nextPolygon();
    void reset();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    double distance2(const QPointF& a, const QPointF& b);
    void drawPolygon(QPainter& painter, const VisualPolygon& poly, const QColor& color, bool active);
    void computeResult();

    VisualPolygon poly1, poly2, result;
    Mode mode;
    PolygonOperationsAlgorithms::Operation operation;
    int movingPoint;
    int currentPolygon;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    PolygonCanvas *canvas;
};

#endif
