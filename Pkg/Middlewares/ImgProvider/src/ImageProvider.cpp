#include "../inc/ImageProvider.h"
#include <QDebug>

ImageProvider::ImageProvider(QObject *parent)
    : QObject(parent), QQuickImageProvider(QQuickImageProvider::Image) {
    image = QImage(224, 224, QImage::Format_RGB32);
    image.fill(QColor("black"));
}

QString extractPathFromUrl(const QString &url) {
    int index = url.indexOf('?');
    if (index != -1) {
        return url.left(index);
    } else {
        return url;
    }
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QString key = extractPathFromUrl(id);
    auto    it  = imgMap.find(key);
    if (it != imgMap.end()) {
        QImage img = it->second;
        *size      = img.size();
        if (requestedSize.isValid() && requestedSize != image.size()) {
            img = img.scaled(requestedSize, Qt::KeepAspectRatio);
        }
        return img;
    }

    return QImage();
}

void ImageProvider::updateImage(const QImage &image, const QString &id) {
    if (!image.isNull() && this->image != image) {
        imgMap[id] = image;
        Q_EMIT imageChanged();
    }
}
