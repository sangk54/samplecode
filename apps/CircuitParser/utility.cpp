#include "utility.h"

MLogger::MLogger()
{
    logLevel = LOG_ERROR;
    currentLevel = LOG_ERROR;
}

void MLogger::setLogLevel(int level)
{
    logLevel = level;
}

MLogger &MLogger::operator()(int level)
{
    currentLevel = level;
    return *this;
}

MLogger mdebug;
