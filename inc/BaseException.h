#ifndef _BASE_EXCEPTION_H_
#define _BASE_EXCEPTION_H_

#include <exception>
#include <string>

using str = std::string;

class BaseException : public std::exception {
public:
    BaseException(const str &time, const str &filename, 
                             const size_t &line, const str &class_name, 
                             const str &method_name, const str &info)
    {
        ErrorInfo = "\nTime: " + time + 
                    "\nError in file " + filename + 
                    ", line " + std::to_string(line) + 
                    ", in method " + method_name + 
                    " of class " + class_name + 
                    "\n Info: " + info + "\n";
    }

    const char *what() const noexcept
    {
        return ErrorInfo.c_str();
    }

protected:
    str ErrorInfo;
};

#endif // _BASE_EXCEPTION_H_
