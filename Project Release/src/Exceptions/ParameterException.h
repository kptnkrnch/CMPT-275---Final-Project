#ifndef PARAMETEREXCEPTION_H
#define PARAMETEREXCEPTION_H

#include <exception>
#include <string>

class ParameterException : public std::exception {
private:
    std::string err;

public:

    explicit ParameterException(const std::string str) {
        err = str;
    }

   virtual ~ParameterException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // PRIMARYKEYEXCEPTION
