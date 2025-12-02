#include "delaunay_visualization.h"

DelaunayWidget::DelaunayWidget(QWidget *parent) : QWidget(parent), onlineMode(false) {
    setMouseTracking(true);
    setMinimumSize(800, 600);
}

void DelaunayWidget::clearPoints() {
    points.clear();
    triangles.clear();
    update();
}

void DelaunayWidget::computeDelaunay() {
    if (points.size() < 3) {
        triangles.clear();
        update();
        return;
    }

    std::vector<AlgoPoint> algoPoints;
    for (const auto& point : points) {
        algoPoints.emplace_back(point.pos.x(), point.pos.y());
    }

    std::vector<AlgoTriangle> algoTriangles = DelaunayAlgorithms::computeDelaunay(algoPoints);

    triangles.clear();
    for (const auto& triangle : algoTriangles) {
        triangles.emplace_back(triangle.p1, triangle.p2, triangle.p3);
    }

    update();
}

void DelaunayWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    if (!triangles.empty()) {
        painter.setPen(QPen(Qt::blue, 1));
        painter.setBrush(QBrush(QColor(200, 200, 255, 100)));

        for (const auto& triangle : triangles) {
            QPolygonF polygon;
            polygon << points[triangle.p1].pos
                    << points[triangle.p2].pos
                    << points[triangle.p3].pos;
            painter.drawPolygon(polygon);
        }
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    for (const auto& point : points) {
        painter.drawEllipse(point.pos, 4, 4);
    }

    painter.setPen(QPen(Qt::darkBlue, 2));
    for (const auto& triangle : triangles) {
        painter.drawLine(points[triangle.p1].pos, points[triangle.p2].pos);
        painter.drawLine(points[triangle.p2].pos, points[triangle.p3].pos);
        painter.drawLine(points[triangle.p3].pos, points[triangle.p1].pos);
    }

    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Точек: %1").arg(points.size()));
    painter.drawText(10, 40, QString("Треугольников: %1").arg(triangles.size()));
    painter.drawText(10, 60, onlineMode ? "Режим: Онлайн" : "Режим: Обычный");
}

void DelaunayWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            QPointF diff = point.pos - pos;
            if (diff.x() * diff.x() + diff.y() * diff.y() <= 100) {
                point.isDragging = true;
                if (onlineMode) {
                    computeDelaunay();
                }
                return;
            }
        }

        points.emplace_back(pos);
        if (onlineMode) {
            computeDelaunay();
        }
        update();
    }
}

void DelaunayWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            if (point.isDragging) {
                point.pos = pos;
                if (onlineMode) {
                    computeDelaunay();
                }
                update();
                return;
            }
        }
    }
}

void DelaunayWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (auto& point : points) {
            point.isDragging = false;
        }
        if (!onlineMode) {
            computeDelaunay();
        }
        update();
    }
}

void DelaunayWidget::setOnlineMode(bool enabled) {
    onlineMode = enabled;
    if (onlineMode && points.size() >= 3) {
        computeDelaunay();
    }
    update();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    delaunayWidget = new DelaunayWidget(this);
    mainLayout->addWidget(delaunayWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QPushButton *clearButton = new QPushButton("Очистить", this);
    QPushButton *computeButton = new QPushButton("Триангуляция Делоне", this);
    QCheckBox *onlineCheckbox = new QCheckBox("Онлайн режим", this);
    QLabel *infoLabel = new QLabel("ЛКМ: добавить точку | Перетащить: двигать точку", this);

    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(computeButton);
    controlLayout->addWidget(onlineCheckbox);
    controlLayout->addWidget(infoLabel);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    connect(clearButton, &QPushButton::clicked, delaunayWidget, &DelaunayWidget::clearPoints);
    connect(computeButton, &QPushButton::clicked, delaunayWidget, &DelaunayWidget::computeDelaunay);
    connect(onlineCheckbox, &QCheckBox::toggled, delaunayWidget, &DelaunayWidget::setOnlineMode);

    setWindowTitle("Триангуляция Делоне");
    resize(900, 700);
}
