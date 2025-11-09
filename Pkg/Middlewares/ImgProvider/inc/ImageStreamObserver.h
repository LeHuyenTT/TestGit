#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QImage>
#include <QObject>
#include <mutex>

class VideoStreamer : public QObject {
    Q_OBJECT
public:
    static VideoStreamer &getInstance() {
        static std::mutex           mutex;
        std::lock_guard<std::mutex> lock(mutex);
        static VideoStreamer        instance;
        return instance;
    }

Q_SIGNALS:
    void newImage(const QImage &image, const QString &id);
    void loginSuccess();

private:
    VideoStreamer();
    ~VideoStreamer();
    VideoStreamer(const VideoStreamer &) = delete;
    VideoStreamer &operator=(const VideoStreamer &) = delete;
};

#endif // VIDEOSTREAMER_H
