#ifndef NOTEXISTSEXCEPTION_H
#define NOTEXISTSEXCEPTION_H

#include <exception>
#include <string>

class NotExistsException : public std::exception {
private:
    std::string err;

public:

    explicit NotExistsException(const std::string str) {
        err = str;
    }

   virtual ~NotExistsException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }
};

#endif // NOTEXISTSEXCEPTION_H

