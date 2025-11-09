#ifndef HTTPCLIENTIMPL_H
#define HTTPCLIENTIMPL_H
#include "../Interfaces/httpclient.h"
#include <QtNetwork>
#include <iostream>
#include <memory>
#include <mutex>
//#include <QObject>
#include <QtCore/QObject>

class HttpClientImpl : public HttpClient {
private:
    HttpClientImpl();
    ~HttpClientImpl();

    //
    static HttpClientImpl *m_instance;
    static std::mutex      m_ctx;
    HttpClientImpl(const HttpClientImpl &) = delete;
    HttpClientImpl &operator=(const HttpClientImpl &) = delete;

public:
    //
    static HttpClientImpl *getInstance();

    // Send a GET request to the specified URL
    void sendGetRequest(QUrl url, std::function<void(QByteArray)> callback) override;

    // Send a POST request to the specified URL
    void sendPostRequest(QUrl url, QJsonObject json,
                         std::function<void(QByteArray)> callback) override;

    // Send a PUT request to the specified URL
    void sendPutRequest(QUrl url, QJsonObject json,
                        std::function<void(QByteArray)> callback) override;

    // Send a DELETE request to the specified URL
    void sendDeleteRequest(QUrl url, std::function<void(QByteArray)> callback) override;
};

#endif // HTTPCLIENTIMPL_H
