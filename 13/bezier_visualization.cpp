#include "bezier_visualization.h"

BezierWidget::BezierWidget(QWidget *parent) : QWidget(parent),
    showQuadratic(true), showCubic(true), showNthOrder(true), currentOrder(5) {
    setMouseTracking(true);
    setMinimumSize(1000, 700);
}

void BezierWidget::clearPoints() {
    points.clear();
    quadraticCurve.clear();
    cubicCurve.clear();
    nthOrderCurve.clear();
    update();
}

void BezierWidget::updateCurves() {
    if (points.size() < 2) {
        quadraticCurve.clear();
        cubicCurve.clear();
        nthOrderCurve.clear();
        update();
        return;
    }

    std::vector<AlgoPoint> algoPoints;
    for (const auto& point : points) {
        algoPoints.emplace_back(point.pos.x(), point.pos.y());
    }

    if (showQuadratic && points.size() >= 3) {
        std::vector<AlgoPoint> quadPoints;
        for (int i = 0; i < std::min(3, (int)points.size()); i++) {
            quadPoints.push_back(algoPoints[i]);
        }
        auto quadResult = BezierAlgorithms::computeBezierQuadratic(quadPoints, 100);
        quadraticCurve.clear();
        for (const auto& p : quadResult) {
            quadraticCurve.emplace_back(p.x, p.y);
        }
    } else {
        quadraticCurve.clear();
    }

    if (showCubic && points.size() >= 4) {
        std::vector<AlgoPoint> cubicPoints;
        for (int i = 0; i < std::min(4, (int)points.size()); i++) {
            cubicPoints.push_back(algoPoints[i]);
        }
        auto cubicResult = BezierAlgorithms::computeBezierCubic(cubicPoints, 100);
        cubicCurve.clear();
        for (const auto& p : cubicResult) {
            cubicCurve.emplace_back(p.x, p.y);
        }
    } else {
        cubicCurve.clear();
    }

    if (showNthOrder && points.size() >= currentOrder + 1) {
        std::vector<AlgoPoint> nthPoints;
        for (int i = 0; i < std::min(currentOrder + 1, (int)points.size()); i++) {
            nthPoints.push_back(algoPoints[i]);
        }
        auto nthResult = BezierAlgorithms::computeBezierNthOrder(nthPoints, 100, currentOrder);
        nthOrderCurve.clear();
        for (const auto& p : nthResult) {
            nthOrderCurve.emplace_back(p.x, p.y);
        }
    } else {
        nthOrderCurve.clear();
    }

    update();
}

void BezierWidget::setOrder(int order) {
    currentOrder = order;
    updateCurves();
}

void BezierWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    for (size_t i = 1; i < points.size(); i++) {
        painter.drawLine(points[i-1].pos, points[i].pos);
    }

    if (showQuadratic && !quadraticCurve.empty()) {
        painter.setPen(QPen(Qt::red, 3));
        for (size_t i = 1; i < quadraticCurve.size(); i++) {
            painter.drawLine(quadraticCurve[i-1], quadraticCurve[i]);
        }
    }

    if (showCubic && !cubicCurve.empty()) {
        painter.setPen(QPen(Qt::green, 3));
        for (size_t i = 1; i < cubicCurve.size(); i++) {
            painter.drawLine(cubicCurve[i-1], cubicCurve[i]);
        }
    }

    if (showNthOrder && !nthOrderCurve.empty()) {
        painter.setPen(QPen(Qt::blue, 3));
        for (size_t i = 1; i < nthOrderCurve.size(); i++) {
            painter.drawLine(nthOrderCurve[i-1], nthOrderCurve[i]);
        }
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    for (const auto& point : points) {
        painter.drawEllipse(point.pos, 5, 5);
    }

    painter.setPen(Qt::black);
    painter.drawText(10, 20, QString("Точек: %1").arg(points.size()));
    painter.drawText(10, 40, QString("Квадратичная: %1").arg(showQuadratic ? "Да" : "Нет"));
    painter.drawText(10, 60, QString("Кубическая: %1").arg(showCubic ? "Да" : "Нет"));
    painter.drawText(10, 80, QString("%1-го порядка: %2").arg(currentOrder).arg(showNthOrder ? "Да" : "Нет"));
}

void BezierWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            QPointF diff = point.pos - pos;
            if (diff.x() * diff.x() + diff.y() * diff.y() <= 100) {
                point.isDragging = true;
                return;
            }
        }

        points.emplace_back(pos);
        updateCurves();
    }
}

void BezierWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF pos = event->position();

        for (auto& point : points) {
            if (point.isDragging) {
                point.pos = pos;
                updateCurves();
                return;
            }
        }
    }
}

void BezierWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (auto& point : points) {
            point.isDragging = false;
        }
    }
}

void BezierWidget::setShowQuadratic(bool show) {
    showQuadratic = show;
    updateCurves();
}

void BezierWidget::setShowCubic(bool show) {
    showCubic = show;
    updateCurves();
}

void BezierWidget::setShowNthOrder(bool show) {
    showNthOrder = show;
    updateCurves();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    bezierWidget = new BezierWidget(this);
    mainLayout->addWidget(bezierWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QPushButton *clearButton = new QPushButton("Очистить", this);
    QCheckBox *quadraticCheck = new QCheckBox("Квадратичная (2)", this);
    QCheckBox *cubicCheck = new QCheckBox("Кубическая (3)", this);
    QCheckBox *nthOrderCheck = new QCheckBox("Высокий порядок", this);
    QLabel *orderLabel = new QLabel("Порядок:", this);

    quadraticCheck->setChecked(true);
    cubicCheck->setChecked(true);
    nthOrderCheck->setChecked(true);

    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(quadraticCheck);
    controlLayout->addWidget(cubicCheck);
    controlLayout->addWidget(nthOrderCheck);
    controlLayout->addWidget(orderLabel);

    QPushButton *order3Btn = new QPushButton("3", this);
    QPushButton *order4Btn = new QPushButton("4", this);
    QPushButton *order5Btn = new QPushButton("5", this);
    QPushButton *order6Btn = new QPushButton("6", this);

    controlLayout->addWidget(order3Btn);
    controlLayout->addWidget(order4Btn);
    controlLayout->addWidget(order5Btn);
    controlLayout->addWidget(order6Btn);

    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    connect(clearButton, &QPushButton::clicked, bezierWidget, &BezierWidget::clearPoints);
    connect(quadraticCheck, &QCheckBox::toggled, bezierWidget, &BezierWidget::setShowQuadratic);
    connect(cubicCheck, &QCheckBox::toggled, bezierWidget, &BezierWidget::setShowCubic);
    connect(nthOrderCheck, &QCheckBox::toggled, bezierWidget, &BezierWidget::setShowNthOrder);

    connect(order3Btn, &QPushButton::clicked, [this]() { bezierWidget->setOrder(3); });
    connect(order4Btn, &QPushButton::clicked, [this]() { bezierWidget->setOrder(4); });
    connect(order5Btn, &QPushButton::clicked, [this]() { bezierWidget->setOrder(5); });
    connect(order6Btn, &QPushButton::clicked, [this]() { bezierWidget->setOrder(6); });

    setWindowTitle("Кривые Безье разных порядков");
    resize(1100, 800);
}
