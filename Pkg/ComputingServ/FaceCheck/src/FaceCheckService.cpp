#include "../inc/FaceCheckService.h"
#include "Logger/inc/logger.h"
#define DEBUG //! comment this line to disable debug mode

FaceCheckService *FaceCheckService::instance = nullptr;
std::mutex        FaceCheckService::m_ctx;

static inline float CosineDistance(const cv::Mat &v1, const cv::Mat &v2) {
    double dot      = v1.dot(v2);
    double denom_v1 = norm(v1);
    double denom_v2 = norm(v2);
    return dot / (denom_v1 * denom_v2);
}

FaceCheckService *FaceCheckService::getInstance() {
    if (instance == nullptr) {
        std::lock_guard<std::mutex> lock(m_ctx);
        if (instance == nullptr) {
            instance = new FaceCheckService();
        }
    }
    return instance;
}

std::string FaceCheckService::setPatternStr(std::string id) {
    this->pattern_jpg = this->path_to_dir.append(id).append("*").append(this->endswith);
    return this->pattern_jpg;
}

bool FaceCheckService::init(std::string id) {
    bool ret = true;
    this->setPatternStr(id);
    this->loadDatabase();
#ifndef DEBUG
    this->getResources(id);
#endif
    return ret;
}

bool FaceCheckService::loadDatabase() {
    if (pattern_jpg.empty()) {
        LOG(LogLevel::ERROR, "pattern_jpg is empty");
        return false;
    }

    // 🔹 Đọc cả JPG và PNG
    std::vector<cv::String> allFiles;
    cv::glob(this->path_to_dir + "/*.jpg", allFiles, false);
    std::vector<cv::String> pngFiles;
    cv::glob(this->path_to_dir + "/*.png", pngFiles, false);
    allFiles.insert(allFiles.end(), pngFiles.begin(), pngFiles.end());
    this->NameFaces = allFiles;

    this->faceCnt = static_cast<int>(this->NameFaces.size());
    if (this->faceCnt == 0) {
        LOG(LogLevel::ERROR, "❌ No image files [jpg/png] in database");
        return false;
    }

    LOG(LogLevel::INFO, "Found " + std::to_string(this->faceCnt) + " pictures in database.");

    this->fc1.clear();
    int validFaces = 0;

    for (int i = 0; i < this->faceCnt; i++) {
        cv::Mat img = cv::imread(this->NameFaces[i], cv::IMREAD_UNCHANGED);
        if (img.empty()) {
            LOG(LogLevel::WARNING, "⚠️ Cannot read image: " + this->NameFaces[i]);
            continue;
        }

        // 🔧 Chuyển đổi ảnh về định dạng BGR 3 kênh nếu cần
        if (img.channels() == 4) {
            cv::cvtColor(img, img, cv::COLOR_BGRA2BGR);
            LOG(LogLevel::INFO, "Converted RGBA -> BGR: " + this->NameFaces[i]);
        } else if (img.channels() == 1) {
            cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
            LOG(LogLevel::INFO, "Converted GRAY -> BGR: " + this->NameFaces[i]);
        }

        // 🧠 Kiểm tra kích thước ảnh
        if (img.cols < 50 || img.rows < 50) {
            LOG(LogLevel::WARNING, "⚠️ Image too small, skip: " + this->NameFaces[i]);
            continue;
        }

        // 🧩 Trích xuất đặc trưng khuôn mặt
        cv::Mat feat = this->ArcFace.GetFeature(img);

        double n = cv::norm(feat);
        if (feat.empty() || cv::countNonZero(feat) == 0 || std::isnan(n) || std::isinf(n)) {
            LOG(LogLevel::WARNING, "⚠️ Invalid or empty feature vector for: " + this->NameFaces[i]);
            continue;
        }

        this->fc1.push_back(feat);
        validFaces++;
        if (faceCnt > 1)
            printf("\rloading: %.2lf%% ", (i * 100.0) / (faceCnt - 1));
    }

    this->faceCnt = validFaces;

    if (this->faceCnt == 0) {
        LOG(LogLevel::ERROR, "❌ No valid faces could be loaded into feature database");
        return false;
    }

    LOG(LogLevel::INFO, "✅ Loaded " + std::to_string(this->faceCnt) + " valid faces in total");
    return true;
}


static inline bool check_create_folder(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        LOG(LogLevel::INFO, "Creating directory " + path);
        int status = mkdir(path.c_str(), 0775);
        if (status != 0) {
            LOG(LogLevel::ERROR, "Failed to create directory " + path);
            return false;
        }
    } else if (info.st_mode & S_IFDIR) {
        LOG(LogLevel::INFO, path + " already exists.");
    } else {
        LOG(LogLevel::ERROR, path + " exists, but is not a directory.");
        return false;
    }

    return true;
}

bool FaceCheckService::getResources(const std::string &id) {
#ifdef DEBUG
    const std::string dir_path_app      = "../";
    const std::string dir_path_database = "../database/";
#else
    const std::string dir_path_app      = "/tmp/app-teacher/";
    const std::string dir_path_database = "/tmp/app-teacher/database/";
#endif
    if (check_create_folder(dir_path_app)) {
        if (check_create_folder(dir_path_database)) {
            // download file
            std::string url = id;
            system("wget -P /tmp/app-teacher/database/ URL");
        }
    }
    return true;
}

bool FaceCheckService::loadModelCustom() {
    bool ret = true;
    return ret;
}

bool FaceCheckService::recognize(QString id, int cam_num) {
    //    std::string pass_id_unuse = id;
    bool    ret = false;
    cv::Mat frame;
    cv::Mat result_cnn;
    //    cv::Mat faces;
    std::vector<FaceObject> Faces;
    int                     failureCnt = 0;
    int                     successCnt = 0;

    //cv::VideoCapture cap(cam_num); // camera
    cv::VideoCapture cap("/workspaces/TestGit/test/Huyen.mp4"); // dùng video file
    if (!cap.isOpened()) {
        //        LOG(LogLevel::ERROR, "Unable to open the camera");
        return 0;
    }

    while (1) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "End of movie" << endl;
            break;
        }
        cv::resize(frame, result_cnn, Size(this->RetinaWidth, this->RetinaHeight), INTER_LINEAR);
#ifdef RETINA
        Rtn->detect_retinaface(result_cnn, Faces);
#else
        MtCNN.detect(result_cnn, Faces);
#endif // RETINA
        if (Faces.size() > 1) {
            LOG(LogLevel::WARNING, "Many faces in camera");
            failureCnt++;
            continue;
            // return 0;
        } else if (Faces.size() < 1) {
            LOG(LogLevel::WARNING, "No face in camera");
            failureCnt++;
            continue;
            // return 0;
        } else {
            if (Faces[0].FaceProb > MIN_FACE_THRESHOLD) {
                cv::Mat aligned = Warp.Process(result_cnn, Faces[0]);
                Faces[0].Angle  = Warp.Angle;
                // features of camera image
                cv::Mat fc2 = ArcFace.GetFeature(aligned);
                // reset indicators
                if (this->faceCnt > 0) {
                    vector<double> score_;
                    for (int c = 0; c < faceCnt; c++)
                        score_.push_back(CosineDistance(fc1[c], fc2));
                    // find the best match
                    double max_score = *max_element(score_.begin(), score_.end());
                    LOG(LogLevel::INFO, "Cosine similarity score: " + std::to_string(max_score));
                    if (max_score > MIN_FACE_THRESHOLD) {
                        successCnt++;
                        if (successCnt + failureCnt > 100) {
                            if (successCnt > 90) {
                                ret = true;
                                ;
                                break;
                            } else {
                                ret = false;

                                break;
                            }
                        }
                        continue;
                    }
                }
            } else {
                failureCnt++;
                continue;
            }
        }
    }
    return ret;
}

bool FaceCheckService::loginWithFace(const Mat &frame) {
    cv::Mat result_cnn;
    //    cv::Mat faces;
    std::vector<FaceObject> Faces;

    if (frame.empty()) {
        LOG(LogLevel::ERROR, "loginWithFace() --> frame is empty");
        return false;
    }
    cv::resize(frame, result_cnn, Size(this->RetinaWidth, this->RetinaHeight), INTER_LINEAR);
#ifdef RETINA
    Rtn->detect_retinaface(result_cnn, Faces);
#else
    MtCNN.detect(result_cnn, Faces);
#endif // RETINA
    if (Faces.size() != 1) {
        LOG(LogLevel::WARNING, "Many faces or no face in camera");
        return false;
    } else {
        //! check face fake or real
        float              x1      = Faces[0].rect.x;
        float              y1      = Faces[0].rect.y;
        float              x2      = Faces[0].rect.width + x1;
        float              y2      = Faces[0].rect.height + y1;
        struct LiveFaceBox LiveBox = {x1, y1, x2, y2};

        float rateFake = Live.Detect(result_cnn, LiveBox);
        if (rateFake <= FACE_LIVING) {
            LOG(LogLevel::WARNING, "Face is fake");
            return false;
        } else {
            LOG(LogLevel::INFO, "Face is real");
        }
        if (Faces[0].FaceProb > MIN_FACE_THRESHOLD) {
            cv::Mat aligned = Warp.Process(result_cnn, Faces[0]);
            Faces[0].Angle  = Warp.Angle;
            // features of camera image
            cv::Mat fc2 = ArcFace.GetFeature(aligned);
            // reset indicators
            if (this->faceCnt > 0) {
                vector<double> score_;
                for (int c = 0; c < faceCnt; c++)
                    score_.push_back(CosineDistance(fc1[c], fc2));
                // find the best match
                double max_score = *max_element(score_.begin(), score_.end());
                if (max_score > MIN_FACE_THRESHOLD) {
                    LOG(LogLevel::INFO, "Max face score: " + std::to_string(max_score));
                    return true;
                }
            }
        }
    }
    return false;
}

FaceCheckService::FaceCheckService(QObject *parent) : QObject(parent) {
    this->Live.LoadModel();
    this->Rtn = new TRetina(this->RetinaWidth, this->RetinaHeight, true);
}

FaceCheckService::~FaceCheckService() {
    if (this->Rtn != nullptr)
        delete this->Rtn;
}

cv::Mat FaceCheckService::getFeatureFromImage(const cv::Mat &img) {
    return this->ArcFace.GetFeature(img);
}