#include "point_in_hull_visualization.h"

PolygonWidget::PolygonWidget(QWidget *parent) : QWidget(parent), onlineMode(false), testPointDragging(false), hullBuilt(false), result(-1) {
    setMouseTracking(true);
    setMinimumSize(800, 600);
}

void PolygonWidget::clearAll() {
    points.clear();
    convexHull.clear();
    convexHullPoints.clear();
    testPoint = QPointF();
    testPointDragging = false;
    hullBuilt = false;
    result = -1;
    update();
}

void PolygonWidget::buildConvexHull() {
    if (points.size() < 3) return;

    std::vector<AlgoPoint2D> algorithmPoints;
    for (const auto& p : points) {
        algorithmPoints.emplace_back(p.x(), p.y());
    }

    convexHullPoints = ConvexHullAlgorithms::compute(algorithmPoints);
    convexHull.clear();
    for (const auto& p : convexHullPoints) {
        convexHull << QPointF(p.x, p.y);
    }
    hullBuilt = true;
    update();
}

void PolygonWidget::checkPoint() {
    if (!hullBuilt || convexHullPoints.empty()) return;

    AlgoPoint2D test(testPoint.x(), testPoint.y());
    result = PointInPolygonAlgorithms::check(test, convexHullPoints);
    update();
}

void PolygonWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    if (hullBuilt && convexHull.size() >= 3) {
        QColor fillColor(200, 200, 255, 100);
        painter.setPen(QPen(Qt::blue, 2));
        painter.setBrush(fillColor);
        painter.drawPolygon(convexHull);
    }

    painter.setPen(Qt::black);
    painter.setBrush(Qt::blue);
    for (const auto& point : points) {
        painter.drawEllipse(point, 4, 4);
    }

    if (!testPoint.isNull()) {
        QColor pointColor;
        if (result == 0) pointColor = Qt::red;
        else if (result == 1) pointColor = Qt::green;
        else if (result == 2) pointColor = QColor(255, 165, 0);
        else pointColor = Qt::gray;

        painter.setPen(Qt::black);
        painter.setBrush(pointColor);
        painter.drawEllipse(testPoint, 6, 6);
    }

    painter.setPen(Qt::black);
    QString status;
    if (result == 0) status = "Снаружи";
    else if (result == 1) status = "Внутри";
    else if (result == 2) status = "На границе";
    else status = "Не проверено";

    painter.drawText(10, 20, QString("Точек: %1 | %2").arg(points.size()).arg(status));
    painter.drawText(10, 40, onlineMode ? "Режим: Онлайн" : "Режим: Обычный");
}

void PolygonWidget::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->position();

    if (event->button() == Qt::LeftButton) {
        if (!testPoint.isNull()) {
            QPointF diff = testPoint - pos;
            if (diff.x() * diff.x() + diff.y() * diff.y() <= 100) {
                testPointDragging = true;
                if (onlineMode && hullBuilt) {
                    checkPoint();
                }
                return;
            }
        }

        if (event->type() == QEvent::MouseButtonDblClick) {
            buildConvexHull();
        } else {
            points.push_back(pos);
            update();
        }
    }
}

void PolygonWidget::mouseMoveEvent(QMouseEvent *event) {
    if (testPointDragging && (event->buttons() & Qt::LeftButton)) {
        testPoint = event->position();
        if (onlineMode && hullBuilt) {
            checkPoint();
        }
        update();
    }
}

void PolygonWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        testPointDragging = false;
        if (!onlineMode && hullBuilt) {
            checkPoint();
        }
    }
}

void PolygonWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!hullBuilt) {
            buildConvexHull();
        } else {
            testPoint = event->position();
            checkPoint();
        }
    }
}

void PolygonWidget::setOnlineMode(bool enabled) {
    onlineMode = enabled;
    if (onlineMode && hullBuilt && !testPoint.isNull()) {
        checkPoint();
    }
    update();
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    polygonWidget = new PolygonWidget(this);
    mainLayout->addWidget(polygonWidget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QPushButton *clearButton = new QPushButton("Очистить", this);
    QPushButton *hullButton = new QPushButton("Построить оболочку", this);
    QCheckBox *onlineCheckbox = new QCheckBox("Онлайн режим", this);
    QLabel *infoLabel = new QLabel("ЛКМ: точки | Двойной: оболочка/проверка | Перетащить: тест-точку", this);

    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(hullButton);
    controlLayout->addWidget(onlineCheckbox);
    controlLayout->addWidget(infoLabel);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    connect(clearButton, &QPushButton::clicked, polygonWidget, &PolygonWidget::clearAll);
    connect(hullButton, &QPushButton::clicked, polygonWidget, &PolygonWidget::buildConvexHull);
    connect(onlineCheckbox, &QCheckBox::toggled, polygonWidget, &PolygonWidget::setOnlineMode);

    setWindowTitle("Точка в выпуклой оболочке");
    resize(900, 700);
}
