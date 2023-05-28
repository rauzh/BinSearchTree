#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include "BaseException.h"

class MemoryException : public BaseException
{
public:
    MemoryException(const str &time, const str &filename,
                    const size_t &line, const str &class_name,
                    const str &method_name,
                    const str &info = "Memory allocation error.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};

class EmptyTreeException : public BaseException
{
public:
    EmptyTreeException(const str &time, const str &filename,
                         const size_t &line, const str &class_name,
                         const str &method_name,
                         const str &info = "Empty tree error.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};

class OutOfRangeException : public BaseException
{
public:
    OutOfRangeException(const str &time, const str &filename,
                        const size_t &line, const str &class_name,
                        const str &method_name,
                        const str &info = "Index is out of range.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};

class NotEqualSizesException : public BaseException
{
public:
    NotEqualSizesException(const str &time, const str &filename,
                           const size_t &line, const str &class_name,
                           const str &method_name,
                           const str &info = "Different size of trees error.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};


class ExpiredException : public BaseException
{
public:
    ExpiredException(const str &time, const str &filename,
                     const size_t &line, const str &class_name,
                     const str &method_name,
                     const str &info = "An attempt was made to access an expired weak pointer.")
        : BaseException(time, filename, line, class_name, method_name, info){};
};

#endif // _EXCEPTIONS_H_
