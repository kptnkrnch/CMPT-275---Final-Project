#ifndef NULLPOINTEREXCEPTION
#define NULLPOINTEREXCEPTION

#include <exception>
#include <string>

class NullPointerException : public std::exception {
private:
    std::string err;

public:

    explicit NullPointerException(const std::string str) {
        err = str;
    }

   virtual ~NullPointerException() throw() {}

    virtual const char* what() const throw() {
        return err.c_str();
    }
};

#endif // NULLPOINTEREXCEPTION

