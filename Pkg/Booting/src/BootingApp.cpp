#include "Booting/inc/BootingApp.h"
#include "Common/constant.h"
#include "Http/inc/httpclientimpl.h"
#include "Logger/inc/logger.h"

namespace fs = std::experimental::filesystem;

// 🧩 Hàm phụ: tách id_stt từ URL (vd: 20521422_1 từ https://ik.imagekit.io/.../20521422_1.jpg?updatedAt=xxx)
static std::string extract_id_stt(const std::string &url) {
    // Lấy phần tên file sau dấu '/'
    size_t lastSlash = url.find_last_of('/');
    if (lastSlash == std::string::npos) return "";

    std::string filename = url.substr(lastSlash + 1);

    // Bỏ phần query sau '?'
    size_t questionMark = filename.find('?');
    if (questionMark != std::string::npos)
        filename = filename.substr(0, questionMark);

    // Bỏ phần đuôi .jpg hoặc .png
    size_t dot = filename.find('.');
    if (dot != std::string::npos)
        filename = filename.substr(0, dot);

    return filename;
}

// 🧩 Hàm chính: clean và tải DB
inline static bool downloadDB(QJsonArray arrLinkDB) {
    bool ret = true;
    try {
        const std::string dbPath = "/workspaces/TestGit/DB";

        // 🧹 Xóa sạch thư mục DB nếu có
        std::string cleanCmd = "rm -rf " + dbPath + " && mkdir -p " + dbPath;
        system(cleanCmd.c_str());
        LOG(LogLevel::INFO, "🧹 Cleaned old DB folder and recreated: " + dbPath);

        // 📥 Tải ảnh mới
        for (int j = 0; j < arrLinkDB.size(); j++) {
            QUrl url = QUrl::fromUserInput(arrLinkDB.at(j).toString());
            std::string urlStr = url.toString().toStdString();

            // ✅ Tách id_stt từ URL
            std::string id_stt = extract_id_stt(urlStr);

            // ✅ Tải ảnh về thư mục DB
            std::string cmd = "wget -q -O " + dbPath + "/" + id_stt + ".png \"" + urlStr + "\"";

            LOG(LogLevel::INFO, "Downloading: " + urlStr);
            LOG(LogLevel::INFO, "→ id_stt: " + id_stt);

            int res = system(cmd.c_str());
            if (res != 0) {
                LOG(LogLevel::ERROR, "❌ Failed to download image: " + urlStr);
                ret = false;
            }
        }

        LOG(LogLevel::INFO, "✅ All images saved to " + dbPath);
    } catch (const std::exception &e) {
        LOG(LogLevel::ERROR, "Download DB failed: " + std::string(e.what()));
        ret = false;
    }
    return ret;
}

// ⚙️ Phần còn lại của BootApp giữ nguyên
BootApp::BootApp() {}

BootApp &BootApp::getInstance() {
    static BootApp instance;
    return instance;
}

bool BootApp::init() {
    LOG(LogLevel::INFO, "Initializing BootApp...");

    const std::string url_str = URL_GET_ALL_IMAGES;
    QUrl url = QUrl::fromUserInput(QString::fromStdString(url_str));
    LOG(LogLevel::INFO, "Requesting image list from: " + url.toString().toStdString());

    bool success = false;

    auto onResponse = [&](QByteArray response) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            LOG(LogLevel::ERROR, "Invalid JSON response.");
            return;
        }

        QJsonObject root = jsonDoc.object();
        if (!root.contains("images") || !root["images"].isArray()) {
            LOG(LogLevel::ERROR, "No 'images' field in response.");
            return;
        }

        QJsonArray dataArr = root["images"].toArray();
        LOG(LogLevel::INFO, "Found " + std::to_string(dataArr.size()) + " images.");

        LOG(LogLevel::INFO, "Downloading DB files (" + std::to_string(dataArr.size()) + ")");
        if (!downloadDB(dataArr)) {
            LOG(LogLevel::ERROR, "Download DB failed!");
            return;
        }

        LOG(LogLevel::INFO, "Download DB done!");
        success = true;
    };

    try {
        HttpClientImpl::getInstance()->sendGetRequest(url, onResponse);
    } catch (const std::exception &e) {
        LOG(LogLevel::ERROR, "Exception during HTTP request: " + std::string(e.what()));
    }

    return success;
}

BootApp::~BootApp() {}
