#ifndef QUERYEXCEPTION_H
#define QUERYEXCEPTION_H

#include <exception>
#include <string>

class QueryException : public std::exception {
private:
    std::string err;

public:

    explicit QueryException(const std::string str) {
        err = str;
    }

    virtual ~QueryException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // QUERYEXCEPTION_H
