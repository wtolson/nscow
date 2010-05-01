#ifndef DRAWBOX_H
#define DRAWBOX_H
#include <QLabel>

class DrawBox : public QLabel
{
Q_OBJECT
public:
    explicit DrawBox(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void clickAt(double x, double y);
    void dragTo(double x, double y);
    void releaseAt(double x, double y);

public slots:

};

#endif // DRAWBOX_H
