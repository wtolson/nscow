#include <QDesktopWidget>
#include <QCursor>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread = new RenderThread();

    QDesktopWidget desktop;
    QRect space = desktop.availableGeometry();
    QPixmap tempPixmap(1,1);
    tempPixmap.fill(qRgb(0,0,0));
    tempPixmap = tempPixmap.scaled(0.75*space.width(), 0.75*space.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->drawBox->setPixmap(tempPixmap);

    connect(ui->stopButton, SIGNAL(clicked()), thread, SLOT(stopProcess()));
    connect(thread, SIGNAL(finished()), this, SLOT(resetUi()));
    connect(thread, SIGNAL(sendImage(QImage)), this, SLOT(updatePixmap(QImage)));
    connect(ui->outputBox, SIGNAL(currentIndexChanged(int)), thread,SLOT(setOutput(int)));

    connect(ui->drawBox, SIGNAL(clickAt(double,double)), thread, SLOT(startForce(double,double)));
    connect(ui->drawBox, SIGNAL(dragTo(double,double)), thread, SLOT(moreForce(double,double)));

    ui->nuSlider->setValue(2);
    ui->dtSlider->setValue(2);
    ui->dxSlider->setValue(2);
    ui->eSlider->setValue(2);
    ui->rToolSlider->setValue(2);
    ui->magToolSlider->setValue(2);
    ui->dcSlider->setValue(2);

    ui->nuSlider->setValue(1*10);
    ui->dtSlider->setValue(1*100);
    ui->dxSlider->setValue(1*100);    
    ui->dcSlider->setValue(1*10);
    ui->rToolSlider->setValue(1);
    ui->magToolSlider->setValue(1);
    ui->eSlider->setValue(0*10);

    ui->drawBox->setCursor(QCursor(QPixmap(":/img/Logo_physics.png").scaled(32,32)));
}

MainWindow::~MainWindow()
{
    delete thread;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::run() {
    ui->runButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->xdimValue->setEnabled(false);
    ui->ydimValue->setEnabled(false);

    bool xokay, yokay;
    int xdim = ui->xdimValue->text().toInt(&xokay);
    int ydim = ui->ydimValue->text().toInt(&yokay);
    if (xokay && yokay) {
        thread->startSim(xdim,ydim);
    } else {
        ui->xdimValue->setText(QString("50"));
        ui->ydimValue->setText(QString("50"));
        thread->startSim(50,50);
    }
}

void MainWindow::updatePixmap(const QImage &image)
{
    if(!image.isNull()){
        QPixmap pixmap = QPixmap::fromImage(image);

        QDesktopWidget desktop;
        QRect space = desktop.availableGeometry();
        pixmap = pixmap.scaled(0.75*space.width(), 0.75*space.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->drawBox->setPixmap(pixmap);
    }
}

void MainWindow::resetUi()
{
    ui->runButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->xdimValue->setEnabled(true);
    ui->ydimValue->setEnabled(true);
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this,tr("Purple Cow"),
                       tr("Navier-Stokes's Cow is a simple fluid dynamics simulator.  It assumes constant density and no-slip boundary conditions.\n\n"
                          "The purple cow was written by William Olson Copyright 2010 and released under the GNU GPL."));
}

void MainWindow::on_nuSlider_valueChanged(int val) {
    ui->nuValue->setText(QString::number(val/10.0));
    thread->setNu(val/10.0);
}

void MainWindow::on_dtSlider_valueChanged(int val) {
    ui->dtValue->setText(QString::number(val/100.0));
    thread->setDt(val/100.0);
}

void MainWindow::on_dxSlider_valueChanged(int val) {
    ui->dxValue->setText(QString::number(val/100.0));
    thread->setDx(val/100.0);
}

void MainWindow::on_eSlider_valueChanged(int val) {
    ui->eValue->setText(QString::number(val/100.0));
    thread->setE(val/100.0);
}

void MainWindow::on_rToolSlider_valueChanged(int val) {
    ui->rToolValue->setText(QString::number(val));
    thread->setRTool(val);
}

void MainWindow::on_magToolSlider_valueChanged(int val) {
    ui->magToolValue->setText(QString::number(val));
    thread->setMagTool(val);
}

void MainWindow::on_dcSlider_valueChanged(int val) {
    ui->dcValue->setText(QString::number(val/10.0));
    thread->setDC(val/10.0);
}

void MainWindow::on_actionAdd_Force_triggered(bool isChecked) {
    if(isChecked) {
        resetTools();
        ui->drawBox->setCursor(QCursor(QPixmap(":/img/Logo_physics.png").scaled(32,32)));

        connect(ui->drawBox, SIGNAL(clickAt(double,double)), thread, SLOT(startForce(double,double)));
        connect(ui->drawBox, SIGNAL(dragTo(double,double)), thread, SLOT(moreForce(double,double)));
    }
    ui->actionAdd_Force->setChecked(true);
}

void MainWindow::on_actionAdd_Dye_triggered(bool isChecked) {
    if(isChecked) {
        resetTools();
        ui->drawBox->setCursor(QCursor(QPixmap(":/img/Beakerblue.png").scaled(32,32)));

        connect(ui->drawBox, SIGNAL(clickAt(double,double)), thread, SLOT(addDye(double,double)));
        connect(ui->drawBox, SIGNAL(dragTo(double,double)), thread, SLOT(addDye(double,double)));
        connect(ui->drawBox, SIGNAL(releaseAt(double,double)), thread, SLOT(stopAction(double,double)));
    }
    ui->actionAdd_Dye->setChecked(true);
}

void MainWindow::on_actionOutward_Force_triggered(bool isChecked) {
    if(isChecked) {
        resetTools();
        ui->drawBox->setCursor(QCursor(QPixmap(":/img/Arrow_out.png").scaled(32,32)));

        connect(ui->drawBox, SIGNAL(clickAt(double,double)), thread, SLOT(addOutForce(double,double)));
        connect(ui->drawBox, SIGNAL(dragTo(double,double)), thread, SLOT(addOutForce(double,double)));
        connect(ui->drawBox, SIGNAL(releaseAt(double,double)), thread, SLOT(stopAction(double,double)));
    }
    ui->actionOutward_Force->setChecked(true);
}

void MainWindow::on_actionInward_Force_triggered(bool isChecked) {
    if(isChecked) {
        resetTools();
        ui->drawBox->setCursor(QCursor(QPixmap(":/img/Arrow_in.png").scaled(32,32)));

        connect(ui->drawBox, SIGNAL(clickAt(double,double)), thread, SLOT(addInForce(double,double)));
        connect(ui->drawBox, SIGNAL(dragTo(double,double)), thread, SLOT(addInForce(double,double)));
        connect(ui->drawBox, SIGNAL(releaseAt(double,double)), thread, SLOT(stopAction(double,double)));
    }
    ui->actionInward_Force->setChecked(true);
}

void MainWindow::resetTools() {
    ui->actionInward_Force->setChecked(false);
    ui->actionAdd_Force->setChecked(false);
    ui->actionAdd_Dye->setChecked(false);
    ui->actionOutward_Force->setChecked(false);

    disconnect(ui->drawBox,SIGNAL(clickAt(double,double)),0,0);
    disconnect(ui->drawBox,SIGNAL(dragTo(double,double)),0,0);
    disconnect(ui->drawBox,SIGNAL(releaseAt(double,double)),0,0);
}
