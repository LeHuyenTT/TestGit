#include "../inc/FaceCheckService.h"
#include "Logger/inc/logger.h"
#define DEBUG //! comment this line to disable debug mode

FaceCheckService *FaceCheckService::instance = nullptr;
std::mutex        FaceCheckService::m_ctx;

static inline float CosineDistance(const cv::Mat &v1, const cv::Mat &v2) {
    double dot      = v1.dot(v2);
    double denom_v1 = norm(v1);
    double denom_v2 = norm(v2);
    if (denom_v1 == 0 || denom_v2 == 0)
        return 0.0f;
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
    this->pattern_jpg = this->path_to_dir + "/" + id + "*" + this->endswith;
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
    cv::Mat faces;
    if (pattern_jpg.empty()) {
        LOG(LogLevel::ERROR, "pattern_jpg is empty");
        return false;
    }
    cv::glob(this->pattern_jpg, this->NameFaces);
    this->faceCnt = this->NameFaces.size();
    if (this->faceCnt == 0) {
        LOG(LogLevel::ERROR, "No image files[jpg] in database");
        return false;
    } else {
        LOG(LogLevel::INFO, "Found " + std::to_string(this->faceCnt) + " pictures in database.");
        for (int i = 0; i < this->faceCnt; i++) {
            // convert to landmark vector and store into fc
            faces = cv::imread(this->NameFaces[i]);
            if (faces.empty()) {
                LOG(LogLevel::WARNING, "⚠️ Cannot read image: " + this->NameFaces[i]);
                continue;
            }

            // 🔧 Ép kích thước ảnh đúng chuẩn model ArcFace
            cv::resize(faces, faces, cv::Size(112, 112));

            // 🧩 Trích xuất đặc trưng
            cv::Mat feat = this->ArcFace.GetFeature(faces);

            // 🔍 Log kiểm tra feature
            double n = cv::norm(feat);
            LOG(LogLevel::INFO, "Feature norm for " + this->NameFaces[i] + ": " + std::to_string(n));

            if (feat.empty() || std::isnan(n) || n == 0) {
                LOG(LogLevel::WARNING, "⚠️ Invalid feature (empty or NaN) for: " + this->NameFaces[i]);
                continue;
            }

            this->fc1.push_back(feat);

            if (faceCnt > 1)
                printf("\rloading: %.2lf%% ", (i * 100.0) / (faceCnt - 1));
        }
        LOG(LogLevel::INFO, "Loaded " + std::to_string(this->faceCnt) + " faces in total");
        return true;
    }
    return false;
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

                // 🔧 Resize ảnh đầu vào đúng chuẩn model ArcFace
                cv::resize(aligned, aligned, cv::Size(112, 112));

                // 🧩 Trích xuất đặc trưng khuôn mặt từ camera
                cv::Mat fc2 = ArcFace.GetFeature(aligned);

                // 🔍 Log kiểm tra feature
                double n2 = cv::norm(fc2);
                LOG(LogLevel::INFO, "Camera feature norm: " + std::to_string(n2));

                if (fc2.empty() || std::isnan(n2) || n2 == 0) {
                    LOG(LogLevel::WARNING, "⚠️ Invalid camera feature vector (empty or NaN). Skipping frame.");
                    failureCnt++;
                    continue;
                }

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
    LOG(LogLevel::INFO, "✅ FaceCheckService initialized (ArcFace assumed pre-loaded).");
}

FaceCheckService::~FaceCheckService() {
    if (this->Rtn != nullptr)
        delete this->Rtn;
}

cv::Mat FaceCheckService::getFeatureFromImage(const cv::Mat &img) {
    return this->ArcFace.GetFeature(img);
}