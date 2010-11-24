#ifndef FREENECT_H
#define FREENECT_H

#include <libfreenect.h>
#include <QMutex>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QThread>

class Freenect
{
public:
    Freenect();
    ~Freenect();

    bool init(int userDeviceNumber = 0);
    bool process(void);

    QSharedPointer<QByteArray> getRgbData(void);
    QSharedPointer<QByteArray> getDepthData(void);

    int getTiltAngle(void) const;
    void setTiltAngle(int angle);

private:
    static void rgbCallback(freenect_device* device, freenect_pixel* rgb,
                            unsigned int timestamp);
    static void depthCallback(freenect_device* device, void* depth,
                              unsigned int timestamp);

    freenect_context* context;
    freenect_device* device;

    class FreenectThread : public QThread
    {
    public:
        explicit FreenectThread(freenect_device* _device);

    protected:
        virtual void run(void);

        freenect_device* device;
    };
    QScopedPointer<FreenectThread> thread;

    // tilt angle of Kinect camera
    int tiltAngle;

    // rgbd data
    char rgb[FREENECT_RGB_SIZE];
    QMutex rgbMutex;

    char depth[FREENECT_DEPTH_SIZE];
    QMutex depthMutex;

    // accelerometer data
    short ax, ay, az;
    double dx, dy, dz;
};

#endif // FREENECT_H
