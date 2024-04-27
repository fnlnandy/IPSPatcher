#ifndef GUARD_GLOBAL_HPP
#define GUARD_GLOBAL_HPP

#include <iostream>

#define FATAL_ERROR(msg)          \
    {                             \
        std::cerr << msg << "\n"; \
        exit(1);                  \
    }

#define BITS_IN(dataType) (sizeof(dataType) * 8)

#endif // GUARD_GLOBAL_HPP