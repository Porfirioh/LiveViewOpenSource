#ifndef REMOTECAMERA_H
#define REMOTECAMERA_H

#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <queue>
#include <vector>
#include <array>
#include <algorithm>
#include <chrono>
#include <atomic>

#include <QDebug>
#include <QDir>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QTcpSocket>

#include "alphanum.hpp"

#include "osutils.h"
#include "cameramodel.h"
#include "constants.h"
#include "lvframe.h"

#define TIMEOUT_DURATION 100

using namespace std::chrono;

class RemoteCamera : public CameraModel
{
    Q_OBJECT

public:
    RemoteCamera(int frWidth = 640,
              int frHeight = 480,
              int dataHeight = 480,
              int Descriptor = 0,
              QObject *parent = nullptr);
    ~RemoteCamera();

    // No setDir function because the source is the server (one place)

    virtual uint16_t* getFrame();
    void PopFrame();
    bool RequestFrame();

public slots:
    void SocketStateChanged(QTcpSocket::SocketState state = QTcpSocket::UnconnectedState);
    void SocketReady();
private:

    QTcpSocket *socket;

    bool is_receiving; // Flag that is true while requesting a frame
    bool is_connected; // Flag that is true when server is connected
    const int nFrames;
    size_t framesize;
    const int headsize;

    size_t image_no;
    std::queue<uint16_t> receive_q;
    std::vector<uint16_t> dummy;
    std::vector<std::atomic<uint16_t>> temp_frame (framesize);

    QFuture<void> readLoopFuture;
    int tmoutPeriod;

};

#endif // REMOTECAMERA_H
