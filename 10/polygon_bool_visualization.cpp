#include "polygon_bool_visualization.h"
#include <QKeyEvent>

PolygonWidget::PolygonWidget(QWidget *parent) : QWidget(parent),
    currentMode(MODE_POLY1), currentOp(OP_UNION),
    currentContourIdx(0), movingPointIdx(-1), movingContourIdx(-1),
    showFill(true), showGrid(false) {

    setMouseTracking(true);
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);

    poly1Contours.push_back(VisualContour());
    poly2Contours.push_back(VisualContour());
}

void PolygonWidget::setOperation(Operation op) {
    currentOp = op;
    if (currentMode == MODE_RESULT) {
        computeResult();
        update();
    }
}

void PolygonWidget::closeCurrentContour() {
    std::vector<VisualContour>* contours = nullptr;

    if (currentMode == MODE_POLY1 && currentContourIdx < poly1Contours.size()) {
        contours = &poly1Contours;
    } else if (currentMode == MODE_POLY2 && currentContourIdx < poly2Contours.size()) {
        contours = &poly2Contours;
    }

    if (contours) {
        auto& contour = (*contours)[currentContourIdx];
        if (contour.points.size() >= 3) {
            contour.closed = true;
        }
    }
    update();
}

void PolygonWidget::nextMode() {
    if (currentMode == MODE_POLY1) {
        currentMode = MODE_POLY2;
    } else if (currentMode == MODE_POLY2) {
        currentMode = MODE_RESULT;
        computeResult();
    } else {
        currentMode = MODE_POLY1;
    }
    currentContourIdx = 0;
    update();
}

void PolygonWidget::newContour() {
    if (currentMode == MODE_POLY1) {
        poly1Contours.push_back(VisualContour());
        currentContourIdx = poly1Contours.size() - 1;
    } else if (currentMode == MODE_POLY2) {
        poly2Contours.push_back(VisualContour());
        currentContourIdx = poly2Contours.size() - 1;
    }
    update();
}

void PolygonWidget::reset() {
    poly1Contours.clear();
    poly2Contours.clear();
    resultContours.clear();

    poly1Contours.push_back(VisualContour());
    poly2Contours.push_back(VisualContour());

    currentMode = MODE_POLY1;
    currentContourIdx = 0;
    movingPointIdx = -1;
    movingContourIdx = -1;

    update();
}

void PolygonWidget::toggleFill(bool fill) {
    showFill = fill;
    update();
}

void PolygonWidget::toggleShowGrid(bool show) {
    showGrid = show;
    update();
}

void PolygonWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Белый фон
    painter.fillRect(rect(), Qt::white);

    // Определяем цвета для каждой операции
    QColor poly1Color, poly2Color, resultColor;

    switch (currentOp) {
    case OP_UNION:
        poly1Color = QColor(100, 150, 255, 100);    // Синий
        poly2Color = QColor(255, 150, 100, 100);    // Оранжевый
        resultColor = QColor(100, 255, 100, 200);   // Ярко-зеленый
        break;
    case OP_INTERSECTION:
        poly1Color = QColor(100, 150, 255, 100);    // Синий
        poly2Color = QColor(255, 150, 100, 100);    // Оранжевый
        resultColor = QColor(255, 255, 100, 200);   // Ярко-желтый
        break;
    case OP_DIFFERENCE:
        poly1Color = QColor(100, 150, 255, 100);    // Синий
        poly2Color = QColor(255, 150, 100, 100);    // Оранжевый
        resultColor = QColor(255, 100, 150, 200);   // Ярко-розовый
        break;
    }

    if (currentMode != MODE_RESULT) {
        // Режим редактирования - только контуры
        for (size_t i = 0; i < poly1Contours.size(); ++i) {
            bool active = (currentMode == MODE_POLY1 && i == currentContourIdx);
            drawContour(painter, poly1Contours[i], poly1Color, active);
        }

        for (size_t i = 0; i < poly2Contours.size(); ++i) {
            bool active = (currentMode == MODE_POLY2 && i == currentContourIdx);
            drawContour(painter, poly2Contours[i], poly2Color, active);
        }

    } else {
        // РЕЖИМ РЕЗУЛЬТАТА - наглядно показываем операцию

        // 1. Рисуем исходные полигоны прозрачными
        painter.setOpacity(0.3);
        for (const auto& contour : poly1Contours) {
            if (contour.closed && contour.points.size() >= 3) {
                QPolygonF poly;
                for (const auto& p : contour.points) {
                    poly << p;
                }
                painter.setPen(QPen(poly1Color.darker(), 1));
                painter.setBrush(QBrush(poly1Color));
                painter.drawPolygon(poly);
            }
        }

        for (const auto& contour : poly2Contours) {
            if (contour.closed && contour.points.size() >= 3) {
                QPolygonF poly;
                for (const auto& p : contour.points) {
                    poly << p;
                }
                painter.setPen(QPen(poly2Color.darker(), 1));
                painter.setBrush(QBrush(poly2Color));
                painter.drawPolygon(poly);
            }
        }

        // 2. Рисуем результат операции БОЛЬШИМ ЯРКИМ ЦВЕТОМ
        painter.setOpacity(1.0);
        drawPolygonContours(painter, resultContours, resultColor);

        // 3. Обводим контуры исходных полигонов
        painter.setPen(QPen(poly1Color.darker(), 2));
        for (const auto& contour : poly1Contours) {
            if (contour.closed && contour.points.size() >= 3) {
                for (size_t i = 0; i < contour.points.size(); ++i) {
                    size_t next = (i + 1) % contour.points.size();
                    painter.drawLine(contour.points[i], contour.points[next]);
                }
            }
        }

        painter.setPen(QPen(poly2Color.darker(), 2));
        for (const auto& contour : poly2Contours) {
            if (contour.closed && contour.points.size() >= 3) {
                for (size_t i = 0; i < contour.points.size(); ++i) {
                    size_t next = (i + 1) % contour.points.size();
                    painter.drawLine(contour.points[i], contour.points[next]);
                }
            }
        }
    }

    // Информация в углу
    painter.setOpacity(1.0);
    painter.setPen(Qt::black);

    QString opText;
    QColor opColor;

    switch (currentOp) {
    case OP_UNION:
        opText = "ОБЪЕДИНЕНИЕ";
        opColor = QColor(100, 255, 100);
        break;
    case OP_INTERSECTION:
        opText = "ПЕРЕСЕЧЕНИЕ";
        opColor = QColor(255, 255, 100);
        break;
    case OP_DIFFERENCE:
        opText = "РАЗНОСТЬ";
        opColor = QColor(255, 100, 150);
        break;
    }

    // Показываем текущую операцию цветным квадратом
    painter.setBrush(QBrush(opColor));
    painter.drawRect(10, 10, 15, 15);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(30, 22, opText);

    QString modeText;
    switch (currentMode) {
    case MODE_POLY1: modeText = "Рисуем Многоугольник 1 (синий)"; break;
    case MODE_POLY2: modeText = "Рисуем Многоугольник 2 (оранжевый)"; break;
    case MODE_RESULT: modeText = "Показываем результат"; break;
    }

    painter.drawText(10, 45, modeText);
}

void PolygonWidget::drawContour(QPainter& painter, const VisualContour& contour,
                                const QColor& color, bool active) {
    if (contour.points.empty()) return;

    QPen pen(active ? color.darker(150) : color.darker(100));
    pen.setWidth(active ? 3 : 2);
    if (!contour.closed) pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    if (showFill && contour.closed && contour.points.size() >= 3) {
        QPolygonF poly;
        for (const auto& p : contour.points) {
            poly << p;
        }
        painter.setBrush(QBrush(color));
        painter.drawPolygon(poly);
    } else {
        painter.setBrush(Qt::NoBrush);
        if (contour.points.size() > 1) {
            for (size_t i = 1; i < contour.points.size(); ++i) {
                painter.drawLine(contour.points[i-1], contour.points[i]);
            }
        }
        if (contour.closed && contour.points.size() >= 3) {
            painter.drawLine(contour.points.back(), contour.points.front());
        }
    }

    // Точки
    painter.setBrush(active ? Qt::white : color.darker(150));
    painter.setPen(Qt::black);
    for (const auto& p : contour.points) {
        painter.drawEllipse(p, active ? 5 : 4, active ? 5 : 4);
    }
}

void PolygonWidget::drawPolygonContours(QPainter& painter,
                                        const std::vector<PolygonContour>& contours,
                                        const QColor& color) {
    if (contours.empty()) return;

    // Яркая толстая обводка для результата
    painter.setPen(QPen(color.darker(), 3));

    for (const auto& contour : contours) {
        if (contour.points.size() < 3) continue;

        QPolygonF poly;
        for (const auto& p : contour.points) {
            poly << QPointF(p.x, p.y);
        }

        if (showFill) {
            // Разные стили заливки для разных операций
            QBrush brush(color);
            if (currentOp == OP_INTERSECTION) {
                // Для пересечения - диагональная штриховка
                brush.setStyle(Qt::DiagCrossPattern);
            } else if (currentOp == OP_DIFFERENCE) {
                // Для разности - вертикальная штриховка
                brush.setStyle(Qt::VerPattern);
            }
            painter.setBrush(brush);
            painter.drawPolygon(poly);
        } else {
            painter.setBrush(Qt::NoBrush);
            painter.drawPolygon(poly);
        }
    }
}

void PolygonWidget::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->pos();

    std::vector<VisualContour>* contours = nullptr;
    if (currentMode == MODE_POLY1) contours = &poly1Contours;
    else if (currentMode == MODE_POLY2) contours = &poly2Contours;
    else return;

    if (event->button() == Qt::RightButton) {
        for (size_t ci = 0; ci < contours->size(); ++ci) {
            auto& contour = (*contours)[ci];
            for (size_t pi = 0; pi < contour.points.size(); ++pi) {
                QPointF diff = contour.points[pi] - pos;
                if (diff.x() * diff.x() + diff.y() * diff.y() < 100) {
                    contour.points.erase(contour.points.begin() + pi);
                    if (contour.points.size() < 3) contour.closed = false;
                    update();
                    return;
                }
            }
        }
        return;
    }

    if (event->button() == Qt::LeftButton) {
        for (size_t ci = 0; ci < contours->size(); ++ci) {
            auto& contour = (*contours)[ci];
            for (size_t pi = 0; pi < contour.points.size(); ++pi) {
                QPointF diff = contour.points[pi] - pos;
                if (diff.x() * diff.x() + diff.y() * diff.y() < 100) {
                    movingPointIdx = pi;
                    movingContourIdx = ci;
                    return;
                }
            }
        }

        if (currentContourIdx < contours->size()) {
            auto& contour = (*contours)[currentContourIdx];
            contour.points.push_back(pos);
            update();
        }
    }
}

void PolygonWidget::mouseMoveEvent(QMouseEvent *event) {
    if (movingPointIdx >= 0 && movingContourIdx >= 0) {
        QPointF pos = event->pos();

        std::vector<VisualContour>* contours = nullptr;
        if (currentMode == MODE_POLY1) contours = &poly1Contours;
        else if (currentMode == MODE_POLY2) contours = &poly2Contours;

        if (contours && movingContourIdx < contours->size()) {
            auto& contour = (*contours)[movingContourIdx];
            if (movingPointIdx < contour.points.size()) {
                contour.points[movingPointIdx] = pos;
                update();
            }
        }
    }
}

void PolygonWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        movingPointIdx = -1;
        movingContourIdx = -1;
    }
}

void PolygonWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Space:
        newContour();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        nextMode();
        break;
    case Qt::Key_R:
        reset();
        break;
    case Qt::Key_C:
        closeCurrentContour();
        break;
    }
}

std::vector<PolygonContour> PolygonWidget::convertToBoolContours(
    const std::vector<VisualContour>& visualContours) {

    std::vector<PolygonContour> result;

    for (const auto& visualContour : visualContours) {
        if (visualContour.closed && visualContour.points.size() >= 3) {
            PolygonContour contour;
            for (const auto& p : visualContour.points) {
                contour.points.push_back(BoolPoint(p.x(), p.y()));
            }
            result.push_back(contour);
        }
    }

    return result;
}

void PolygonWidget::computeResult() {
    auto boolPoly1 = convertToBoolContours(poly1Contours);
    auto boolPoly2 = convertToBoolContours(poly2Contours);

    PolygonBoolean::Operation boolOp;
    switch (currentOp) {
    case OP_UNION:
        boolOp = PolygonBoolean::UNION;
        break;
    case OP_INTERSECTION:
        boolOp = PolygonBoolean::INTERSECTION;
        break;
    case OP_DIFFERENCE:
        boolOp = PolygonBoolean::DIFFERENCE;
        break;
    }

    resultContours = PolygonBoolean::booleanOperation(boolPoly1, boolPoly2, boolOp);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    widget = new PolygonWidget(this);
    mainLayout->addWidget(widget);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    QPushButton *modeBtn = new QPushButton("След. режим (Enter)", this);
    QPushButton *contourBtn = new QPushButton("Новый контур (Space)", this);
    QPushButton *resetBtn = new QPushButton("Сброс (R)", this);
    QPushButton *closeBtn = new QPushButton("Закрыть контур (C)", this);

    QRadioButton *unionRadio = new QRadioButton("Объединение", this);
    QRadioButton *intersectRadio = new QRadioButton("Пересечение", this);
    QRadioButton *diffRadio = new QRadioButton("Разность (1-2)", this);

    QCheckBox *fillCheck = new QCheckBox("Заполнение", this);
    QCheckBox *gridCheck = new QCheckBox("Сетка", this);

    unionRadio->setChecked(true);
    fillCheck->setChecked(true);
    gridCheck->setChecked(false);

    controlLayout->addWidget(modeBtn);
    controlLayout->addWidget(contourBtn);
    controlLayout->addWidget(resetBtn);
    controlLayout->addWidget(closeBtn);
    controlLayout->addWidget(unionRadio);
    controlLayout->addWidget(intersectRadio);
    controlLayout->addWidget(diffRadio);
    controlLayout->addWidget(fillCheck);
    controlLayout->addWidget(gridCheck);
    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    connect(modeBtn, &QPushButton::clicked, widget, &PolygonWidget::nextMode);
    connect(contourBtn, &QPushButton::clicked, widget, &PolygonWidget::newContour);
    connect(resetBtn, &QPushButton::clicked, widget, &PolygonWidget::reset);
    connect(closeBtn, &QPushButton::clicked, widget, &PolygonWidget::closeCurrentContour);

    connect(unionRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) widget->setOperation(PolygonWidget::OP_UNION);
    });
    connect(intersectRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) widget->setOperation(PolygonWidget::OP_INTERSECTION);
    });
    connect(diffRadio, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) widget->setOperation(PolygonWidget::OP_DIFFERENCE);
    });

    connect(fillCheck, &QCheckBox::toggled, widget, &PolygonWidget::toggleFill);
    connect(gridCheck, &QCheckBox::toggled, widget, &PolygonWidget::toggleShowGrid);

    setWindowTitle("Булевы операции - Цветная визуализация");
    resize(1000, 700);
}
