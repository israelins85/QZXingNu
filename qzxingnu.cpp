#include "qzxingnu.h"
#include <QCoreApplication>
#include <QtQml/QQmlEngine>
#include "qzxingnufilter.h"
#include <zxing-cpp/core/src/BarcodeFormat.h>
#include <zxing-cpp/core/src/DecodeHints.h>
#include <zxing-cpp/core/src/GenericLuminanceSource.h>
#include <zxing-cpp/core/src/HybridBinarizer.h>
#include <zxing-cpp/core/src/MultiFormatReader.h>
#include <zxing-cpp/core/src/Result.h>

using ZXingFormats = std::vector<ZXing::BarcodeFormat>;
using ZXing::DecodeHints;
using ZXing::GenericLuminanceSource;
using ZXing::HybridBinarizer;
using ZXing::MultiFormatReader;
using ZXing::Result;

static QVector<QPointF> toQVectorOfQPoints(const std::vector<ZXing::ResultPoint>& points)
{
    QVector<QPointF> result;
    for (const auto& point : points) {
        result.append(QPointF(point.x(), point.y()));
    }
    return result;
}

static QZXingNuDecodeResult toQZXingNuDecodeResult(const ZXing::Result& result)
{
    return {static_cast<QZXingNuDecodeResult::DecodeStatus>(result.status()),
            static_cast<QZXingNuDecodeResult::BarcodeFormat>(result.format()),
            QString::fromStdWString(result.text()),
            QByteArray((const char*) result.rawBytes().data(), result.rawBytes().size()),
            toQVectorOfQPoints(result.resultPoints()),
            result.isValid()};
}
static ZXingFormats zxingFormats(const QVector<int>& from)
{
    ZXingFormats result;
    result.reserve(static_cast<ZXingFormats::size_type>(from.size()));
    std::transform(from.begin(), from.end(), std::back_inserter(result), [](int a) {
        return static_cast<ZXing::BarcodeFormat>(a);
    });
    return result;
}

void QZXingNu::registerQMLTypes()
{
    qRegisterMetaType<QZXingNuDecodeResult>("DecodeResult");
    qRegisterMetaType<QZXingNuDecodeResult::DecodeStatus>("DecodeStatus");
    qRegisterMetaType<QZXingNuDecodeResult::BarcodeFormat>("BarcodeFormat");

    qmlRegisterType<QZXingNu>("com.github.swex.QZXingNu", 1, 0, "QZXingNu");
    qmlRegisterType<QZXingNuFilter>("com.github.swex.QZXingNu", 1, 0, "QZXingNuFilter");
}

QZXingNu::QZXingNu(QObject* parent)
    : QObject(parent)
{}

QVector<int> QZXingNu::formats() const
{
    return m_formats;
}

bool QZXingNu::tryHarder() const
{
    return m_tryHarder;
}

bool QZXingNu::tryRotate() const
{
    return m_tryRotate;
}

QZXingNuDecodeResult QZXingNu::decodeImage(QImage&& imagez)
{
    QImage image;
    if (imagez.format() != QImage::Format_Grayscale8) {
        image = imagez.convertToFormat(QImage::Format_Grayscale8);
    } else {
        image = imagez;
    }

    // reentrant
    auto        luminanceSource = std::make_shared<GenericLuminanceSource>(image.width(),
                                                                    image.height(),
                                                                    image.bits(),
                                                                    image.bytesPerLine());
    DecodeHints hints;
    auto        convertFormats = [this]() { return zxingFormats(m_formats); };
    hints.setPossibleFormats(convertFormats());
    hints.setTryHarder(m_tryHarder);
    hints.setTryRotate(m_tryRotate);
    MultiFormatReader reader(hints);
    auto              result = reader.read(HybridBinarizer(luminanceSource));
    if (result.isValid()) {
        auto qzxingResult = toQZXingNuDecodeResult(result);
        return qzxingResult;
    }
    return {};
}

QZXingNuDecodeResult QZXingNu::decodeFrame(const QVideoFrame& frame)
{
    if (!frame.isValid()) {
        return {};
    }
    return decodeImage(frame.toImage());
}

void QZXingNu::setFormats(QVector<int> formats)
{
    if (m_formats == formats)
        return;

    m_formats = formats;
    emit formatsChanged(m_formats);
}

void QZXingNu::setTryHarder(bool tryHarder)
{
    if (m_tryHarder == tryHarder)
        return;

    m_tryHarder = tryHarder;
    emit tryHarderChanged(m_tryHarder);
}

void QZXingNu::setTryRotate(bool tryRotate)
{
    if (m_tryRotate == tryRotate)
        return;

    m_tryRotate = tryRotate;
    emit tryRotateChanged(m_tryRotate);
}
