#pragma once

#include <QByteArray>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>

class QZXingNuDecodeResult
{
    Q_GADGET
    Q_PROPERTY(DecodeStatus status MEMBER status)
    Q_PROPERTY(BarcodeFormat format MEMBER format)
    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(QByteArray rawBytes MEMBER rawBytes)
    Q_PROPERTY(QVector<QPointF> points MEMBER points)
    Q_PROPERTY(bool valid MEMBER valid)

public:
    // ZXing::BarcodeFormat
    enum class BarcodeFormat {
        NONE              = 0, ///< Used as a return value if no valid barcode has been detected
        AZTEC             = (1 << 0),  ///< Aztec (2D)
        CODABAR           = (1 << 1),  ///< CODABAR (1D)
        CODE_39           = (1 << 2),  ///< Code 39 (1D)
        CODE_93           = (1 << 3),  ///< Code 93 (1D)
        CODE_128          = (1 << 4),  ///< Code 128 (1D)
        DATA_MATRIX       = (1 << 5),  ///< Data Matrix (2D)
        EAN_8             = (1 << 6),  ///< EAN-8 (1D)
        EAN_13            = (1 << 7),  ///< EAN-13 (1D)
        ITF               = (1 << 8),  ///< ITF (Interleaved Two of Five) (1D)
        MAXICODE          = (1 << 9),  ///< MaxiCode (2D)
        PDF_417           = (1 << 10), ///< PDF417 (1D) or (2D)
        QR_CODE           = (1 << 11), ///< QR Code (2D)
        RSS_14            = (1 << 12), ///< RSS 14
        RSS_EXPANDED      = (1 << 13), ///< RSS EXPANDED
        UPC_A             = (1 << 14), ///< UPC-A (1D)
        UPC_E             = (1 << 15), ///< UPC-E (1D)
        UPC_EAN_EXTENSION = (1 << 16), ///< UPC/EAN extension (1D). Not a stand-alone format.
    };
    Q_ENUM(BarcodeFormat)

    // ZXing::DecodeStatus
    enum class DecodeStatus {
        NoError = 0,
        NotFound,
        FormatError,
        ChecksumError,
    };
    Q_ENUM(DecodeStatus)

public:
    DecodeStatus     status;
    BarcodeFormat    format;
    QString          text;
    QByteArray       rawBytes;
    QVector<QPointF> points;
    bool             valid;
};

Q_DECLARE_METATYPE(QZXingNuDecodeResult::BarcodeFormat)
Q_DECLARE_METATYPE(QZXingNuDecodeResult::DecodeStatus)
Q_DECLARE_METATYPE(QZXingNuDecodeResult)
