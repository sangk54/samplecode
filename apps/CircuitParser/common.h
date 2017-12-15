#ifndef COMMON_H
#define COMMON_H

#include "utility.h"

extern MLogger mlogger;

#define mAll   mlogger(LOG_ALL)
#define mDebug mlogger(LOG_DEBUG)
#define mError mlogger(LOG_ERROR)
#define mFatal mlogger(LOG_FATAL)
#define mInfo  mlogger(LOG_INFO)

#endif // COMMON_H
