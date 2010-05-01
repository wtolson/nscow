#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QImage>
#include <QMutex>
#include <QThread>

#include "VectorField2D.h"
#include "ScalarField2D.h"

class RenderThread : public QThread
{
    Q_OBJECT
public:
    enum OutputType {
        DENSITY, VELOCITY, PRESSURE, VORTICITY
            };
    enum Actions {
        NONE, FORCE, DYE, INFORCE, OUTFORCE
            };

    RenderThread(QObject *parent = 0);
    ~RenderThread();

    void startSim(int xdim = 50, int ydim = 50);
    void startSim(QImage& image);

signals:
    void sendImage(QImage image);

public slots:
    void stopProcess();

    void startForce(double x, double y);
    void moreForce(double x, double y);
    void addDye(double x, double y);
    void stopAction(double, double);
    void addInForce(double x, double y);\
    void addOutForce(double x, double y);\



    void setNu(double nu);
    void setDt(double dt);
    void setDx(double dx);
    void setE(double e);
    void setRTool(double rForce);
    void setMagTool(double magTool);
    void setDC(double dc);
    void setOutput(int output);

protected:
    void run();

private:
    QMutex mutex;
    bool m_abort;
    QImage m_image;

    int m_hasAction;    
    QPointF m_fstart;
    QPointF m_fend;
    double m_xTool;
    double m_yTool;

    int xdim;
    int ydim;

    int itsOutput;

    static void writeImage(const ScalarField2D& data, QImage& image);
    static void writeImage(const VectorField2D& data, QImage& image);
};

#endif // RENDERTHREAD_H
