#ifndef OPENFILEEXCEPTION_H
#define OPENFILEEXCEPTION_H

#include <exception>
#include <string>

class OpenFileException : public std::exception {
private:
    std::string err;

public:

    explicit OpenFileException(const std::string str) {
        err = str;
    }

    virtual ~OpenFileException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // OPENFILEEXCEPTION_H

