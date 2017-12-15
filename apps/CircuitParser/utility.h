#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>

enum LogType {LOG_ALL, LOG_DEBUG, LOG_ERROR, LOG_FATAL, LOG_INFO, LOG_OFF};

class MLogger
{
public:
    MLogger();
    void setLogLevel(int level);

    template <typename T>
    MLogger& operator<<(const T &t)
    {
        if (currentLevel >= logLevel)
            std::cout << t;

        return *this;
    }

    MLogger& operator()(int level);

private:
    int logLevel;
    int currentLevel;
};

#endif // UTILITY_H
