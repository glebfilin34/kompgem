#ifndef POLYGON_BOOL_VISUALIZATION_H
#define POLYGON_BOOL_VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <vector>
#include "polygon_bool_algorithms.h"

class PolygonWidget : public QWidget {
    Q_OBJECT

public:
    enum Mode { MODE_POLY1, MODE_POLY2, MODE_RESULT };
    enum Operation { OP_UNION, OP_INTERSECTION, OP_DIFFERENCE };
    
    PolygonWidget(QWidget *parent = nullptr);
    
    void setOperation(Operation op);
    void closeCurrentContour();
    
public slots:
    void nextMode();
    void newContour();
    void reset();
    void toggleFill(bool fill);
    void toggleShowGrid(bool show);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    struct VisualContour {
        std::vector<QPointF> points;
        bool closed;
        VisualContour() : closed(false) {}
    };
    
    void drawContour(QPainter& painter, const VisualContour& contour, 
                     const QColor& color, bool active);
    void drawPolygonContours(QPainter& painter, 
                             const std::vector<PolygonContour>& contours,
                             const QColor& color);
    void drawLegend(QPainter& painter, const QColor& color1, 
                   const QColor& color2, const QColor& resultColor,
                   const QString& operation);
    void computeResult();
    std::vector<PolygonContour> convertToBoolContours(
        const std::vector<VisualContour>& visualContours);
    
    std::vector<VisualContour> poly1Contours;
    std::vector<VisualContour> poly2Contours;
    std::vector<PolygonContour> resultContours;
    
    Mode currentMode;
    Operation currentOp;
    int currentContourIdx;
    int movingPointIdx;
    int movingContourIdx;
    
    bool showFill;
    bool showGrid;
};

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    PolygonWidget *widget;
};

#endif