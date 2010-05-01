#include <QMouseEvent>
#include <QPoint>
#include "drawbox.h"

DrawBox::DrawBox(QWidget *parent) :
        QLabel(parent)
{
}

void DrawBox::mousePressEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton)
        clickAt(ev->posF().x()/this->width(),ev->posF().y()/this->height());
}

void DrawBox::mouseMoveEvent(QMouseEvent *ev) {
    if (ev->buttons() & Qt::LeftButton) {
        dragTo(ev->posF().x()/this->width(),ev->posF().y()/this->height());
    }
}

void DrawBox::mouseReleaseEvent(QMouseEvent *ev) {
    releaseAt(ev->posF().x()/this->width(),ev->posF().y()/this->height());
}
