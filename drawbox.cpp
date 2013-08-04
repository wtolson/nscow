#include <QMouseEvent>
#include <QPoint>
#include "drawbox.h"

DrawBox::DrawBox(QWidget *parent) :
        QLabel(parent)
{
}

void DrawBox::mousePressEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton)
        clickAt((float) ev->pos().x()/this->width(), (float) ev->pos().y()/this->height());
}

void DrawBox::mouseMoveEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton) {
        dragTo((float) ev->pos().x()/this->width(), (float) ev->pos().y()/this->height());
    }
}

void DrawBox::mouseReleaseEvent(QMouseEvent *ev) {
    releaseAt((float) ev->pos().x()/this->width(), (float) ev->pos().y()/this->height());
}
