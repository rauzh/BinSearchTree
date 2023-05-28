#ifndef _BASE_ITERATOR_H_
#define _BASE_ITERATOR_H_

#include <cstdlib>

class BaseIterator {

    using size_type = size_t;

public:
    BaseIterator()
    {
        index = 0;
        size = 0;
    }

    BaseIterator(const BaseIterator &iterator)
    {
        index = iterator.index;
        size = iterator.size;
    }
    
    virtual ~BaseIterator() = 0;

protected:
    size_type index = 0;
    size_type size = 0;
};

#endif // _BASE_ITERATOR_H_
