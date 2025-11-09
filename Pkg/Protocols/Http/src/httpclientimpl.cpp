#include "../inc/httpclientimpl.h"

std::mutex      HttpClientImpl::m_ctx;
HttpClientImpl *HttpClientImpl::m_instance = nullptr;

HttpClientImpl::HttpClientImpl() {}

HttpClientImpl::~HttpClientImpl() {
    if (m_instance == nullptr) {
        delete m_instance;
    }
}

HttpClientImpl *HttpClientImpl::getInstance() {
    m_ctx.lock();
    if (!m_instance) {
        m_instance = new HttpClientImpl();
    }
    m_ctx.unlock();
    return m_instance;
}

void HttpClientImpl::sendGetRequest(QUrl url, std::function<void(QByteArray)> callback) {
    QNetworkRequest request(url);
    QNetworkReply * reply = m_manager.get(request);
    QEventLoop      loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray responseData = reply->readAll();
    // invoke the callback function
    callback(responseData);
    //
    reply->deleteLater();
}

void HttpClientImpl::sendPostRequest(QUrl url, QJsonObject json,
                                     std::function<void(QByteArray)> callback) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Convert the JSON object to a QByteArray
    QJsonDocument jsonDoc(json);
    QByteArray    jsonData = jsonDoc.toJson();

    QNetworkReply *reply = m_manager.post(request, jsonData);
    QEventLoop     loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray responseData = reply->readAll();
    // invoke the callback function
    callback(responseData);
    reply->deleteLater();
}

void HttpClientImpl::sendPutRequest(QUrl url, QJsonObject json,
                                    std::function<void(QByteArray)> callback) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Convert the JSON object to a QByteArray
    QJsonDocument jsonDoc(json);
    QByteArray    jsonData = jsonDoc.toJson();

    QNetworkReply *reply = m_manager.put(request, jsonData);
    QEventLoop     loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray responseData = reply->readAll();
    // invoke the callback function
    callback(responseData);
    reply->deleteLater();
}

void HttpClientImpl::sendDeleteRequest(QUrl url, std::function<void(QByteArray)> callback) {
    QNetworkRequest request(url);
    QNetworkReply * reply = m_manager.deleteResource(request);
    QEventLoop      loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray responseData = reply->readAll();
    // invoke the callback function
    callback(responseData);
    reply->deleteLater();
}
