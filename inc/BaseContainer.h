#ifndef _BASE_CONTAINER_H_
#define _BASE_CONTAINER_H_

#include <cstdlib>

class BaseContainer {

    using size_type = size_t;

public:
    BaseContainer()
    {
        size = 0;
    }

    bool IsEmpty() const
    {
        return size == 0;
    }

    size_t Size() const
    {
        return size;
    }

    virtual ~BaseContainer() = 0;

protected:
    size_type size = 0;
};

#endif // _BASE_CONTAINER_H_
