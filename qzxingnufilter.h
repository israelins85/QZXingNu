#pragma once

#include <QElapsedTimer>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QThreadPool>
#include <QVideoSink>
#include <qzxingnu.h>

class QZXingNuFilter : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QZXingNu* qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(int intervalDecode READ intervalDecode WRITE setIntervalDecode NOTIFY intervalDecodeChanged FINAL)

    QZXingNu*     m_qzxingNu  = nullptr;
    QVideoSink*   m_videoSink = nullptr;
    QAtomicInt    m_decodersRunning;
    bool          m_running = true;
    QElapsedTimer m_lastFrameProcess;
    int           m_intervalDecode = 200;

public:
    QZXingNuFilter(QObject* parent = nullptr);

    QZXingNu*   qzxingNu() const;
    QVideoSink* videoSink() const;

    bool running() const;
    void setRunning(bool newRunning);

    int  intervalDecode() const;
    void setIntervalDecode(int newIntervalDecode);

private:
    void frameChanged(const QVideoFrame& frame);

signals:
    void codeRecognized(QString code, QZXingNuDecodeResult::BarcodeFormat format);

public slots:
    void setQzxingNu(QZXingNu* qzxingNu);
    void setVideoSink(QVideoSink* newVideoSink);

signals:
    void qzxingNuChanged(QZXingNu* qzxingNu);
    void decodedResult(QZXingNuDecodeResult result);
    void videoSinkChanged();
    void runningChanged();
    void intervalDecodeChanged();
};
