#include <QColor>
#include <cmath>
using namespace std;

#include "ScalarField2D.h"
#include "VectorField2D.h"
#include "VectorOperators.h"
#include "NavierStokes.h"
#include "renderthread.h"

RenderThread::RenderThread(QObject *parent)
    : QThread(parent), m_abort(false), m_hasAction(NONE), itsOutput(0)
{
}

RenderThread::~RenderThread()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}

void RenderThread::startSim(int xdim, int ydim)
{
    this->xdim = xdim;
    this->ydim = ydim;
    m_abort = false;
    start();
}

void RenderThread::startSim(QImage& image)
{
    if(!image.isNull()) {
        m_image = image;
        xdim = m_image.width();
        ydim = m_image.height();
    } else {
        xdim = 50;
        ydim = 50;
    }
    m_abort = false;
    start();
}

void RenderThread::run()
{
    m_image = QImage(xdim,ydim,QImage::Format_RGB32);

    ScalarField2D p(xdim,ydim);
    p.fill(0.0);

    ScalarField2D rho(xdim,ydim,1);
    rho.fill(0.0);

    VectorField2D v(xdim,ydim,-1);
    v.fill(0.0);

    while(!m_abort) {
        NavierStokes::advect(v);
        NavierStokes::diffuse(v);

        switch (m_hasAction) {
        case FORCE:
            NavierStokes::addForces(v, ydim*m_fstart.y(), xdim*m_fstart.x(), ydim*m_fend.y(), xdim*m_fend.x());
            m_fstart = m_fend;
            m_hasAction = NONE;
            break;
        case DYE:
            NavierStokes::addDye(rho,m_xTool,m_yTool);
            break;
        case INFORCE:
            NavierStokes::addInOutForce(v,m_xTool,m_yTool,-1);
            break;
        case OUTFORCE:
            NavierStokes::addInOutForce(v,m_xTool,m_yTool);
        default:
            break;
        }

        NavierStokes::computePressure(v,p);
        NavierStokes::subtractPressureGradient(v,p);
        NavierStokes::vorticityConfinement(v);

        NavierStokes::advect(v,rho);
        NavierStokes::diffuse(v,rho);

        NavierStokes::t += NavierStokes::dt;

        switch (itsOutput) {
        case DENSITY:
            writeImage(rho,m_image);
            break;
        case VELOCITY:
            writeImage(v,m_image);
            break;
        case PRESSURE:
            writeImage(p,m_image);
            break;
        case VORTICITY:
            ScalarField2D vorticity(xdim,ydim);
            DelCross(vorticity,v);
            writeImage(vorticity,m_image);
            break;
        }
        emit sendImage(m_image);
        msleep(50);
    }
}

void RenderThread::stopProcess()
{
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}

void RenderThread::writeImage(const ScalarField2D& data, QImage& image) {
    int xdim = data.xdim;
    int ydim = data.ydim;

    double scale = 255 / max(abs(data.min()), abs(data.max()));
    QRgb* ppixel = (QRgb*) image.bits();
    for(int x = 0; x < xdim; x++) {
        for(int y = 0; y < ydim; y++) {
            if (data.value(x,y) < 0)
                *ppixel = qRgb(-scale*data.value(x,y),0,0);
            else
                *ppixel = qRgb(0,0,scale*data.value(x,y));
            ppixel++;
        }
    }
}


void RenderThread::writeImage(const VectorField2D& data, QImage& image) {
    int xdim = data.xdim;
    int ydim = data.ydim;

    double scale = 255/max(abs(data.max()),abs(data.min()));;

    QRgb* ppixel = (QRgb*) image.bits();
    for(int x = 0; x < xdim; x++) {
        for(int y = 0; y < ydim; y++) {
            double g = 0.0;
            if (data.value(x,y,0) < 0.0) g -= data.value(x,y,0);
            if (data.value(x,y,1) < 0.0) g -= data.value(x,y,1);
            *ppixel = qRgb(scale * abs( data.value(x,y,0) ) , 0.5*scale*g , scale*abs(data.value(x,y,1)) );
            ppixel++;
        }
    }
}

void RenderThread::startForce(double x, double y) {
    mutex.lock();
    m_fstart = QPointF(x,y);
    mutex.unlock();
}

void RenderThread::moreForce(double x, double y) {
    mutex.lock();
    if (m_fstart.x()!=x && m_fstart.y()!=y) {
        m_fend = QPointF(x,y);
        m_hasAction = FORCE;
    }
    mutex.unlock();
}

void RenderThread::addDye(double x, double y) {
    mutex.lock();
    m_yTool = x*xdim;
    m_xTool = y*ydim;
    m_hasAction = DYE;
    mutex.unlock();
}

void RenderThread::stopAction(double, double) {
    mutex.lock();
    m_hasAction = NONE;
    mutex.unlock();
}

void RenderThread::addInForce(double x, double y) {
    mutex.lock();
    m_yTool = x*xdim;
    m_xTool = y*ydim;
    m_hasAction = INFORCE;
    mutex.unlock();
}

void RenderThread::addOutForce(double x, double y) {
    mutex.lock();
    m_yTool = x*xdim;
    m_xTool = y*ydim;
    m_hasAction = OUTFORCE;
    mutex.unlock();
}

void RenderThread::setNu(double nu) {
    mutex.lock();
    NavierStokes::nu = nu;
    mutex.unlock();
}

void RenderThread::setDt(double dt) {
    mutex.lock();
    NavierStokes::dt = dt;
    mutex.unlock();
}

void RenderThread::setDx(double dx) {
    mutex.lock();
    NavierStokes::dx = dx;
    mutex.unlock();
}

void RenderThread::setE(double e) {
    mutex.lock();
    NavierStokes::e = e;
    mutex.unlock();
}

void RenderThread::setRTool(double rTool) {
    mutex.lock();
    NavierStokes::rTool = rTool;
    mutex.unlock();
}

void RenderThread::setMagTool(double magTool) {
    mutex.lock();
    NavierStokes::magTool = magTool;
    mutex.unlock();
}

void RenderThread::setDC(double dc) {
    mutex.lock();
    NavierStokes::dC = dc;
    mutex.unlock();
}

void RenderThread::setOutput(int output) {
    mutex.lock();
    itsOutput = output;
    mutex.unlock();
}
