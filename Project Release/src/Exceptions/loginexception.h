#ifndef LOGINEXCEPTION
#define LOGINEXCEPTION

#include <exception>
#include <string>

class LoginException : public std::exception {
private:
    std::string err;

public:

    explicit LoginException(const std::string str) {
        err = str;
    }

    virtual ~LoginException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }

};

#endif // LOGINEXCEPTION

