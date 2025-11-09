#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief Define the path to the file
 */
#define PATH_FOLDER_DB "/workspaces/TestGit/DB"
#define PATH_TO_FILE_STARTUP "./startup.txt"

/**
 * @brief Define the path to the file
 */

/**
 * @brief Define the URL of the server
 */
#define URL_SERVER_BACKEND "https://glorious-space-spoon-r44j464qprwc5g4v-9001.app.github.dev"
#define URL_SERVER_SOCKET "http://127.0.0.1:4024"
#define URL_SERVER_RTSP "rtsp://54.253.92.7:8554/"

#define URL_GET_ALL_IMAGES \
    URL_SERVER_BACKEND         \
    "/api/v1/users/images/all"

#define URL_USER_LOGIN \
    URL_SERVER_BACKEND         \
    "/api/v1/users/images/all"
/**
 * @brief Define the index of the peripheral
 */
#define CAMERA_INDEX 0

#endif