#include "qzxingnufilter.h"
#include <QFutureWatcher>
#include <QPointer>
#include <QVariant>
#include <QVideoFrame>
#include <QtConcurrent>
#include <functional>
#include <qzxingnu.h>
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

extern QImage qt_imageFromVideoFrame(const QVideoFrame& frame);

void QZXingNuFilter::processVideoFrame(const QVideoFrame& a_videoFrame) {
    if (m_busy)
        return;

    if (!m_enabled)
        return;

    if (m_lastFrameCheckedTime.isValid() &&
        m_lastFrameCheckedTime.elapsed() < m_intervalToCheckFrames) {
        // skipped frame
        return;
    }

    if (!a_videoFrame.isValid())
        return;

    m_lastFrameCheckedTime.start();
    m_busy = true;
    QThreadPool::globalInstance()->start([this, a_videoFrame]() {
        try {
            QImage      l_image;
            QVideoFrame l_frame(a_videoFrame);

            l_frame.map(QVideoFrame::ReadOnly);
            l_image = l_frame.toImage();
            l_frame.unmap();

            // cropping image to captureRect
            // if (captureRect().isValid()) {
            //     l_image = l_image.copy(captureRect());
            // }

            // processing the image
            auto result = m_qzxingNu->decodeImage(l_image);
        } catch (...) {
            qCritical() << "An error occurred.";
        }
        m_busy = false;
    });
}

void QZXingNuFilter::decoded(QZXingNu::DecodeResult a_result) {
    if (!a_result.valid)
        return;
    if (a_result.status == QZXingNu::DecodeStatus::NoError)
        emit tagFound(a_result.text, a_result.format);
}

QZXingNuFilter::QZXingNuFilter(QObject* parent)
    : QObject(parent) {
}

QZXingNuFilter::~QZXingNuFilter() {
    // disable to stop processing
    m_enabled = false;

    while (m_busy) {
        qApp->processEvents();
        QThread::msleep(10);
    }
}

QVideoSink* QZXingNuFilter::videoSink() const {
    return m_videoSink;
}

void QZXingNuFilter::setVideoSink(QVideoSink* a_videoSink) {
    if (m_videoSink == a_videoSink)
        return;

    if (m_videoSink) {
        disconnect(m_videoSink);
    }

    m_videoSink = a_videoSink;
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &QZXingNuFilter::processVideoFrame);
    emit videoSinkChanged();
}

QRect QZXingNuFilter::captureRect() const {
    return m_captureRect;
}

void QZXingNuFilter::setCaptureRect(const QRect& a_captureRect) {
    if (m_captureRect == a_captureRect)
        return;

    m_captureRect = a_captureRect;
    emit captureRectChanged(m_captureRect);
}

qint32 QZXingNuFilter::intervalToCheckFrames() const {
    return m_intervalToCheckFrames;
}

void QZXingNuFilter::setIntervalToCheckFrames(qint32 a_intervalToCheckFrames) {
    if (m_intervalToCheckFrames == a_intervalToCheckFrames)
        return;

    m_intervalToCheckFrames = a_intervalToCheckFrames;
    emit intervalToCheckFramesChanged(m_intervalToCheckFrames);
}

QZXingNu* QZXingNuFilter::qzxingNu() const {
    return m_qzxingNu;
}

void QZXingNuFilter::setQzxingNu(QZXingNu* a_qzxingNu) {
    if (m_qzxingNu == a_qzxingNu)
        return;

    if (m_qzxingNu) {
        disconnect(this);
    }

    m_qzxingNu = a_qzxingNu;
    connect(m_qzxingNu, &QZXingNu::decodeResultChanged, this, &QZXingNuFilter::decoded);
    emit qzxingNuChanged(m_qzxingNu);
}

bool QZXingNuFilter::enabled() const {
    return m_enabled;
}

void QZXingNuFilter::setEnabled(bool newEnabled) {
    if (m_enabled == newEnabled)
        return;
    m_enabled = newEnabled;
    emit enabledChanged();
}
