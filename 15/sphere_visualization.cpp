#include "sphere_visualization.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <cmath>

SphereWidget::SphereWidget(QWidget *parent) : QWidget(parent),
    rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
    mousePressed(false), autoRotate(true), centerX(400), centerY(300), scale(200.0f) {

    setMinimumSize(800, 600);

    sphereVertices = SphereAlgorithms::generateSphereVertices(1.0f, 24, 12);
    meridians = SphereAlgorithms::generateMeridians(1.0f, 8, 24);
    parallels = SphereAlgorithms::generateParallels(1.0f, 6, 24);

    connect(&animationTimer, &QTimer::timeout, this, &SphereWidget::animate);
    animationTimer.start(16);
}

void SphereWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(20, 20, 40));

    drawSphere(painter);

    painter.setPen(Qt::white);
    painter.drawText(10, 20, "Перетащите мышью для вращения");
    painter.drawText(10, 40, QString("Вращение: X=%1° Y=%2° Z=%3°").arg(rotationX).arg(rotationY).arg(rotationZ));
}

void SphereWidget::drawSphere(QPainter &painter) {
    painter.save();
    painter.translate(centerX, centerY);

    for (const auto& vertex : sphereVertices) {
        SpherePoint rotated = SphereAlgorithms::rotatePoint(vertex.position, rotationX, rotationY, rotationZ);

        QColor color(vertex.color.r * 255, vertex.color.g * 255, vertex.color.b * 255);
        painter.setPen(color);
        painter.setBrush(color);

        int x = rotated.x * scale;
        int y = rotated.y * scale;
        int z = rotated.z * scale;

        int screenX = x;
        int screenY = z;
        int size = 3;

        painter.drawEllipse(QPoint(screenX, screenY), size, size);
    }

    painter.setPen(QPen(Qt::white, 2));
    for (const auto& point : meridians) {
        SpherePoint rotated = SphereAlgorithms::rotatePoint(point, rotationX, rotationY, rotationZ);
        int x = rotated.x * scale;
        int y = rotated.z * scale;
        painter.drawPoint(x, y);
    }

    painter.setPen(QPen(Qt::yellow, 2));
    for (const auto& point : parallels) {
        SpherePoint rotated = SphereAlgorithms::rotatePoint(point, rotationX, rotationY, rotationZ);
        int x = rotated.x * scale;
        int y = rotated.z * scale;
        painter.drawPoint(x, y);
    }

    painter.restore();
}

void SphereWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        mousePressed = true;
    }
}

void SphereWidget::mouseMoveEvent(QMouseEvent *event) {
    if (mousePressed) {
        QPoint delta = event->pos() - lastMousePos;
        rotationY += delta.x() * 0.5f;
        rotationX += delta.y() * 0.5f;
        lastMousePos = event->pos();
        update();
    }
}

void SphereWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    centerX = width() / 2;
    centerY = height() / 2;
    scale = std::min(width(), height()) * 0.2f;
}

void SphereWidget::animate() {
    if (autoRotate) {
        rotationY += 0.5f;
        update();
    }
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    sphereWidget = new SphereWidget(this);
    mainLayout->addWidget(sphereWidget);

    setWindowTitle("Вращающаяся 3D Сфера");
    resize(900, 700);
}
