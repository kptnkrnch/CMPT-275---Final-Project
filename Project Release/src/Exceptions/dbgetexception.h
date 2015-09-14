#ifndef DBGETEXCEPTION_H
#define DBGETEXCEPTION_H

#include <exception>
#include <string>

class DBGetException : public std::exception {
private:
    std::string err;

public:

    explicit DBGetException(const std::string str) {
        err = str;
    }

    virtual ~DBGetException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // DBGETEXCEPTION_H

