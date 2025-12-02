#include "convex_hull_visualization.h"

ConvexHullWidget::ConvexHullWidget(QWidget *parent) : QWidget(parent), onlineMode(false) {
    setMouseTracking(true);
    setMinimumSize(800, 600);
}

void ConvexHullWidget::clearPoints() {
    points.clear();
    convexHull.clear();
    update();
}

void ConvexHullWidget::computeConvexHull() {
    if (points.size() < 3) {
        convexHull.clear();
        update();
        return;
    }

    std::vector<AlgorithmPoint> algorithmPoints;
    for (const auto& point : points) {
        algorithmPoints.emplace_back(point.pos.x(), point.pos.y());
    }

    std::vector<AlgorithmPoint> result = ConvexHullAlgorithms::computeGrahamScan(algorithmPoints);
    convexHull.clear();
    for (const auto& point : result) {
        convexHull.emplace_back(point.x, point.y);
    }

    update();
}

void ConvexHullWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::blue);
    for (const auto& point : points) {
        painter.drawEllipse(point.pos, 5, 5);
    }

    if (convexHull.size() >= 3) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(QBrush(QColor(255, 0, 0, 50)));

        QPolygonF polygon;
        for (const auto& point : convexHull) {
            polygon << point;
        }
        painter.drawPolygon(polygon);
    }

    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Точек: %1").arg(points.size()));
    painter.drawText(10, 40, QString("Вершин оболочки: %1").arg(convexHull.size()));
    painter.drawText(10, 60, onlineMode ? "Режим: Онлайн" : "Режим: Обычный");
}

void ConvexHullWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            QPointF diff = point.pos - pos;
            if (diff.x() * diff.x() + diff.y() * diff.y() <= 100) {
                point.isDragging = true;
                if (onlineMode) {
                    computeConvexHull();
                }
                return;
            }
        }

        points.emplace_back(pos);
        if (onlineMode) {
            computeConvexHull();
        }
        update();
    }
}

void ConvexHullWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            if (point.isDragging) {
                point.pos = pos;
                if (onlineMode) {
                    computeConvexHull();
                }
                update();
                return;
            }
        }
    }
}

void ConvexHullWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (auto& point : points) {
            point.isDragging = false;
        }
        if (!onlineMode) {
            computeConvexHull();
        }
        update();
    }
}

void ConvexHullWidget::setOnlineMode(bool enabled) {
    onlineMode = enabled;
    if (onlineMode) {
        computeConvexHull();
    }
    update();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    convexHullWidget = new ConvexHullWidget(this);
    mainLayout->addWidget(convexHullWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QPushButton *clearButton = new QPushButton("Очистить", this);
    QPushButton *computeButton = new QPushButton("Построить оболочку", this);
    QCheckBox *onlineCheckbox = new QCheckBox("Онлайн режим", this);
    QLabel *infoLabel = new QLabel("ЛКМ: добавить точку | Перетащить: двигать точку", this);

    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(computeButton);
    controlLayout->addWidget(onlineCheckbox);
    controlLayout->addWidget(infoLabel);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    connect(clearButton, &QPushButton::clicked, convexHullWidget, &ConvexHullWidget::clearPoints);
    connect(computeButton, &QPushButton::clicked, convexHullWidget, &ConvexHullWidget::computeConvexHull);
    connect(onlineCheckbox, &QCheckBox::toggled, convexHullWidget, &ConvexHullWidget::setOnlineMode);

    setWindowTitle("Выпуклая оболочка");
    resize(900, 700);
}
