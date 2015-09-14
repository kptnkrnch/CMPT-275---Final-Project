#ifndef USEREXCEPTION
#define USEREXCEPTION

#include <exception>
#include <string>

class UserException : public std::exception {
private:
    std::string err;

public:

    explicit UserException(const std::string str) {
        err = str;
    }

    virtual ~UserException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // USEREXCEPTION

