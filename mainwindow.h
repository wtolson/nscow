#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "renderthread.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updatePixmap(const QImage &image);

protected:
    void changeEvent(QEvent *e);

private slots:
    void run();
    void resetUi();
    void on_actionAbout_triggered();

    void on_nuSlider_valueChanged(int);
    void on_dtSlider_valueChanged(int);
    void on_dxSlider_valueChanged(int);
    void on_eSlider_valueChanged(int);
    void on_rToolSlider_valueChanged(int);
    void on_magToolSlider_valueChanged(int);
    void on_dcSlider_valueChanged(int);

    void on_actionAdd_Force_triggered(bool);
    void on_actionAdd_Dye_triggered(bool);
    void on_actionOutward_Force_triggered(bool);
    void on_actionInward_Force_triggered(bool);

private:
    Ui::MainWindow *ui;
    RenderThread *thread;

    void resetTools();
};

#endif // MAINWINDOW_H
