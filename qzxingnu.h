#pragma once

#include <QImage>
#include <QObject>
#include <QVideoFrame>
#include "qzxingnudecoderesult.h"

class QZXingNu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<int> formats READ formats WRITE setFormats NOTIFY formatsChanged)
    Q_PROPERTY(bool tryHarder READ tryHarder WRITE setTryHarder NOTIFY tryHarderChanged)
    Q_PROPERTY(bool tryRotate READ tryRotate WRITE setTryRotate NOTIFY tryRotateChanged)

public:
    static void registerQMLTypes();

public:
    explicit QZXingNu(QObject* parent = nullptr);
    QVector<int>         formats() const;
    bool                 tryHarder() const;
    bool                 tryRotate() const;
    QZXingNuDecodeResult decodeImage(QImage&& image);
    QZXingNuDecodeResult decodeFrame(const QVideoFrame& image);

signals:
    void imageDecoded(QString data);
    void formatsChanged(QVector<int> formats);
    void tryHarderChanged(bool tryHarder);
    void tryRotateChanged(bool tryRotate);

public slots:
    void setFormats(QVector<int> formats);
    void setTryHarder(bool tryHarder);
    void setTryRotate(bool tryRotate);

private:
    QVector<int>         m_formats;
    bool                 m_tryHarder = false;
    bool                 m_tryRotate = false;
};
