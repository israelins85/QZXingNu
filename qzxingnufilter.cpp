#include "qzxingnufilter.h"
#include "qzxingnu.h"

#include <QFutureWatcher>
#include <QPointer>
#include <QTimer>
#include <QVariant>
#include <QVideoSink>
#include <QtConcurrent>
#include <functional>
#include <memory>

#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

QZXingNuFilter::QZXingNuFilter(QObject* parent)
    : QObject(parent)
{
    // when founded "stop" the decoding
    connect(
        this,
        &QZXingNuFilter::decodedResult,
        this,
        [this](QZXingNuDecodeResult result) {
            // só precisa de um resultado valido
            setRunning(false);
            emit codeRecognized(result.text, result.format);
        },
        Qt::QueuedConnection);
}

QZXingNu* QZXingNuFilter::qzxingNu() const
{
    return m_qzxingNu;
}

QVideoSink* QZXingNuFilter::videoSink() const
{
    return m_videoSink;
}

void QZXingNuFilter::frameChanged(const QVideoFrame& frame)
{
    if (!m_running)
        return;

    if (!m_qzxingNu)
        return;

    if (m_decodersRunning > 0)
        return;

    if (m_lastFrameProcess.isValid() && m_lastFrameProcess.elapsed() < m_intervalDecode)
        return;

    m_lastFrameProcess.start();
    m_decodersRunning++;
    auto future = QtConcurrent::run([this, frame]() {
        auto result = m_qzxingNu->decodeFrame(frame);
        if (result.valid)
            emit decodedResult(result);
        m_decodersRunning--;
    });
    Q_UNUSED(future);
}

void QZXingNuFilter::setQzxingNu(QZXingNu* qzxingNu)
{
    if (m_qzxingNu == qzxingNu)
        return;

    m_qzxingNu = qzxingNu;
    emit qzxingNuChanged(m_qzxingNu);
}

void QZXingNuFilter::setVideoSink(QVideoSink* newVideoSink)
{
    if (m_videoSink == newVideoSink)
        return;

    if (m_videoSink != nullptr) {
        disconnect(m_videoSink, &QVideoSink::videoFrameChanged, this, &QZXingNuFilter::frameChanged);
    }

    m_videoSink = newVideoSink;
    emit videoSinkChanged();

    if (m_videoSink != nullptr) {
        connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &QZXingNuFilter::frameChanged);
    }
}

bool QZXingNuFilter::running() const
{
    return m_running;
}

void QZXingNuFilter::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
    emit runningChanged();
}

int QZXingNuFilter::intervalDecode() const
{
    return m_intervalDecode;
}

void QZXingNuFilter::setIntervalDecode(int newIntervalDecode)
{
    if (m_intervalDecode == newIntervalDecode)
        return;
    m_intervalDecode = newIntervalDecode;
    emit intervalDecodeChanged();
}
