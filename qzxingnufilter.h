#ifndef QZXINGNUFILTER_H
#define QZXINGNUFILTER_H

#include <QThreadPool>
#include <QTime>
#include <QVideoSink>
#include <qzxingnu.h>

class QZXingNuFilter : public QObject {
    Q_OBJECT
    Q_PROPERTY(QZXingNu* qzxingNu READ qzxingNu WRITE setQzxingNu NOTIFY qzxingNuChanged)
    Q_PROPERTY(QRect captureRect READ captureRect WRITE setCaptureRect NOTIFY captureRectChanged)
    Q_PROPERTY(qint32 intervalToCheckFrames READ intervalToCheckFrames WRITE
                   setIntervalToCheckFrames NOTIFY intervalToCheckFramesChanged)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

    QVideoSink*   m_videoSink = nullptr;
    QZXingNu*     m_qzxingNu  = nullptr;
    QRect         m_captureRect;
    QElapsedTimer m_lastFrameCheckedTime;
    qint32        m_intervalToCheckFrames = 1000;

    void processVideoFrame(const QVideoFrame& a_videoFrame);
    void decoded(QZXingNu::DecodeResult a_result);

public:
    QZXingNuFilter(QObject* parent = nullptr);
    ~QZXingNuFilter();

    QVideoSink* videoSink() const;
    void        setVideoSink(QVideoSink* a_videoSink);

    QRect captureRect() const;
    void  setCaptureRect(const QRect& a_captureRect);

    qint32 intervalToCheckFrames() const;
    void   setIntervalToCheckFrames(qint32 a_intervalToCheckFrames);

    QZXingNu* qzxingNu() const;

    bool enabled() const;
    void setEnabled(bool newEnabled);

public slots:
    void setQzxingNu(QZXingNu* a_qzxingNu);

signals:
    void tagFound(QString tag, QZXingNu::BarcodeFormat format);
    void intervalToCheckFramesChanged(qint32 intervalToCheckFrames);
    void captureRectChanged(const QRect& a_captureRect);
    void qzxingNuChanged(QZXingNu* qzxingNu);
    void enabledChanged();
    void videoSinkChanged();

private:
    bool m_enabled = true;
    bool m_busy    = false;
};

#endif // QZXINGNUFILTER_H
