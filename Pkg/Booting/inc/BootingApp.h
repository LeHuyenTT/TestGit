#ifndef BOOTAPP_H
#define BOOTAPP_H

#include <array>
#include <chrono>
#include <ctime>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sys/stat.h>
#include <vector>

class BootApp {
private:

    BootApp();                                    // private constructor to prevent instantiation
    ~BootApp();                                   // private destructor to prevent instantiation
    BootApp(const BootApp &) = delete;            // delete copy constructor
    BootApp &operator=(const BootApp &) = delete; // delete assignment operator

public:
    static BootApp &    getInstance();
    bool                init();
};

#endif // BOOTAPP_H
