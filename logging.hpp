#ifndef LOGGING_H
#define LOGGING_H

#include <cassert>
#include <iostream>

#define ASSERT(_condition_, _msg_) do { \
    if (!(_condition_)) { \
        printf("%s", _msg_); \
        exit(1);  \
    } \
} while(false);

#endif //LOGGING_H
