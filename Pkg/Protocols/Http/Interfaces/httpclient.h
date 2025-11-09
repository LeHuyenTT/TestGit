#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <QNetworkAccessManager>

class HttpClient {
public:
    HttpClient() {}
    virtual ~HttpClient() {}

    // Send a GET request to the specified URL
    virtual void sendGetRequest(QUrl url, std::function<void(QByteArray)> callback) = 0;

    // Send a POST request to the specified URL
    virtual void sendPostRequest(QUrl url, QJsonObject json,
                                 std::function<void(QByteArray)> callback) = 0;

    // Send a PUT request to the specified URL
    virtual void sendPutRequest(QUrl url, QJsonObject json,
                                std::function<void(QByteArray)> callback) = 0;

    // Send a DELETE request to the specified URL
    virtual void sendDeleteRequest(QUrl url, std::function<void(QByteArray)> callback) = 0;

protected:
    QNetworkAccessManager m_manager;
};

#endif // HTTPCLIENT_H
