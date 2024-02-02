#ifndef JSON_PROPERTYEXCEPTION_H
#define JSON_PROPERTYEXCEPTION_H
#include <stdexcept>

struct JSON_PropertyException : public std::runtime_error
{
    JSON_PropertyException(const char* propertyName, const char* what)
        : std::runtime_error(std::string("Error reading property \"") + propertyName + "\" " + what) {}
};

#endif // JSON_PROPERTYEXCEPTION_H
