#include "polygon_ops_visualization.h"

PolygonCanvas::PolygonCanvas(QWidget *parent) : QWidget(parent), mode(FIRST_POLYGON),
    movingPoint(-1), currentPolygon(-1), operation(PolygonOperationsAlgorithms::INTERSECTION) {
    setMouseTracking(true);
}

void PolygonCanvas::setOperation(PolygonOperationsAlgorithms::Operation op) {
    operation = op;
    if (mode == RESULT) {
        computeResult();
        update();
    }
}

void PolygonCanvas::nextPolygon() {
    if (mode == FIRST_POLYGON) {
        mode = SECOND_POLYGON;
    } else if (mode == SECOND_POLYGON) {
        computeResult();
        mode = RESULT;
    }
    update();
}

void PolygonCanvas::reset() {
    poly1.clear();
    poly2.clear();
    result.clear();
    mode = FIRST_POLYGON;
    movingPoint = -1;
    currentPolygon = -1;
    update();
}

void PolygonCanvas::mousePressEvent(QMouseEvent *event) {
    if (mode == RESULT) return;

    QPointF p(event->pos().x(), event->pos().y());

    if (event->button() == Qt::RightButton) {
        if (mode == FIRST_POLYGON || mode == SECOND_POLYGON) {
            VisualPolygon& currentPoly = (mode == FIRST_POLYGON) ? poly1 : poly2;
            for (int i = 0; i < currentPoly.size(); i++) {
                if (distance2(p, currentPoly.points[i].pos) < 100) {
                    currentPoly.points.erase(currentPoly.points.begin() + i);
                    update();
                    return;
                }
            }
        }
        return;
    }

    if (event->button() == Qt::LeftButton) {
        if (mode == FIRST_POLYGON) {
            for (int i = 0; i < poly1.size(); i++) {
                if (distance2(p, poly1.points[i].pos) < 100) {
                    movingPoint = i;
                    currentPolygon = 1;
                    return;
                }
            }
            poly1.addPoint(VisualPoint(p));
        } else if (mode == SECOND_POLYGON) {
            for (int i = 0; i < poly2.size(); i++) {
                if (distance2(p, poly2.points[i].pos) < 100) {
                    movingPoint = i;
                    currentPolygon = 2;
                    return;
                }
            }
            poly2.addPoint(VisualPoint(p));
        }
        update();
    }
}

void PolygonCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (movingPoint >= 0) {
        QPointF p(event->pos().x(), event->pos().y());
        if (currentPolygon == 1) {
            poly1.points[movingPoint].pos = p;
        } else if (currentPolygon == 2) {
            poly2.points[movingPoint].pos = p;
        }
        update();
    }
}

void PolygonCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        movingPoint = -1;
        currentPolygon = -1;
    }
}

void PolygonCanvas::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        nextPolygon();
    }
}

void PolygonCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    if (mode != RESULT) {
        drawPolygon(painter, poly1, Qt::blue, mode == FIRST_POLYGON);
        drawPolygon(painter, poly2, Qt::red, mode == SECOND_POLYGON);
    } else {
        drawPolygon(painter, poly1, Qt::blue, false);
        drawPolygon(painter, poly2, Qt::red, false);
        drawPolygon(painter, result, Qt::green, true);
    }
}

double PolygonCanvas::distance2(const QPointF& a, const QPointF& b) {
    double dx = a.x() - b.x();
    double dy = a.y() - b.y();
    return dx*dx + dy*dy;
}

void PolygonCanvas::drawPolygon(QPainter& painter, const VisualPolygon& poly, const QColor& color, bool active) {
    if (poly.empty()) return;

    QPen pen(color);
    pen.setWidth(active ? 3 : 2);
    painter.setPen(pen);

    for (size_t i = 0; i < poly.points.size(); i++) {
        size_t next = (i + 1) % poly.points.size();
        painter.drawLine(poly.points[i].pos, poly.points[next].pos);
    }

    painter.setBrush(active ? color.lighter(150) : Qt::NoBrush);
    if (poly.points.size() >= 3) {
        QPolygonF qpoly;
        for (const auto& p : poly.points) {
            qpoly << p.pos;
        }
        painter.drawPolygon(qpoly);
    }

    for (const auto& p : poly.points) {
        painter.setBrush(color);
        painter.drawEllipse(p.pos, 5, 5);
    }
}

void PolygonCanvas::computeResult() {
    AlgoPolygon algoPoly1, algoPoly2;

    for (const auto& point : poly1.points) {
        algoPoly1.addPoint(AlgoPoint(point.pos.x(), point.pos.y()));
    }

    for (const auto& point : poly2.points) {
        algoPoly2.addPoint(AlgoPoint(point.pos.x(), point.pos.y()));
    }

    algoPoly1.computeConvexHull();
    algoPoly2.computeConvexHull();

    AlgoPolygon algoResult = PolygonOperationsAlgorithms::computeOperation(algoPoly1, algoPoly2, operation);

    result.clear();
    for (const auto& point : algoResult.points) {
        result.addPoint(VisualPoint(QPointF(point.x, point.y)));
    }
}

MainWindow::MainWindow() {
    setWindowTitle("Polygon Operations");
    setFixedSize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    canvas = new PolygonCanvas(this);
    mainLayout->addWidget(canvas);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *nextButton = new QPushButton("Next Polygon", this);
    QPushButton *resetButton = new QPushButton("Reset", this);

    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(resetButton);

    QRadioButton *intersectionRadio = new QRadioButton("Intersection", this);
    QRadioButton *unionRadio = new QRadioButton("Union", this);
    QRadioButton *differenceRadio = new QRadioButton("Difference", this);

    intersectionRadio->setChecked(true);

    buttonLayout->addWidget(intersectionRadio);
    buttonLayout->addWidget(unionRadio);
    buttonLayout->addWidget(differenceRadio);

    mainLayout->addLayout(buttonLayout);

    connect(nextButton, &QPushButton::clicked, canvas, &PolygonCanvas::nextPolygon);
    connect(resetButton, &QPushButton::clicked, canvas, &PolygonCanvas::reset);

    connect(intersectionRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) canvas->setOperation(PolygonOperationsAlgorithms::INTERSECTION);
    });
    connect(unionRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) canvas->setOperation(PolygonOperationsAlgorithms::UNION);
    });
    connect(differenceRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) canvas->setOperation(PolygonOperationsAlgorithms::DIFFERENCE);
    });
}
