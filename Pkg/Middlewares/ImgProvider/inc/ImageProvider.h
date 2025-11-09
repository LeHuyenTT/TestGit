#ifndef OPENCVIMAGEPROVIDER_H
#define OPENCVIMAGEPROVIDER_H

#include <QImage>
#include <QObject>
#include <QQuickImageProvider>
#include <QString>
#include <iostream>
#include <map>

class ImageProvider : public QObject, public QQuickImageProvider {
    Q_OBJECT
public:
    ImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

public Q_SLOTS:
    void updateImage(const QImage &image, const QString &id);

Q_SIGNALS:
    void imageChanged();

private:
    QImage                    image;
    std::map<QString, QImage> imgMap;
};

#endif // OPENCVIMAGEPROVIDER_H
