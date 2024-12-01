#pragma once
#include "JSON_PropertyException.h"
#include <json_spirit/json_spirit_value.h>

// why are thesenot in the class?
template<typename T>
bool GetValueFromArray(const json_spirit::mArray* array, T& out, int index)
{
    if (!array || index < 0 || array->size() <= index)
        return false;
    out = array->at(index).get_value<T>();
    return true;
}

template<typename T>
bool GetValueFromObject(const json_spirit::mObject* obj, T& out, std::string propertyName)
{
    if (!obj)
        return false;

    auto it = obj->find(propertyName);
    if (it == obj->end())
        return false;

    out = it->second.get_value<T>();
    return true;
}

class JSON_PropertyReader
{
public:
    json_spirit::mObject* sourceObject{}; // 0x00
    json_spirit::mObject* otherSourceObject{}; // 0x08
    json_spirit::mArray* sourceArray{}; // 0x10
    json_spirit::mArray* otherSourceArray{}; // 0x18

    template<typename T>
    bool ReadIfExists(T& out, int index)
    {
        if (index < 0)
            return false;
        if (sourceArray && sourceArray->size() <= index)
            return false;
        if (otherSourceArray && otherSourceArray->size() < index)
            return false;

        Read(out, index);
        return true;
    }

    template<typename T>
    bool ReadIfExists(T& out, std::string propertyName)
    {
        if (!sourceObject && !otherSourceObject)
            return false;
        if (sourceObject->find(propertyName) == sourceObject->end() && otherSourceObject->find(propertyName) == otherSourceObject->end())
            return false;

        bool readingTemp = reading;
        reading = false;
        Read(out, propertyName);
        reading = readingTemp;
        return true;
    }

    template<typename T>
    void Read(T& out, int index)
    {
        ReadFromArray(out, index);
    }

    template<typename T>
    void Read(T& out, std::string propertyName)
    {
        ReadFromObject(out, propertyName);
    }

    void Unload();
private:
    bool reading{}; // 0x20
    double field_24 = 1.0; // 0x24

    template<typename T>
    void ReadFromObject(T& out, std::string propertyName)
    {
        T otherValue;
        bool hasValue = GetValueFromObject(sourceObject, out, propertyName);
        bool hasOtherValue = GetValueFromObject(otherSourceObject, otherValue, propertyName);

        if (hasOtherValue)
            Adjust(out, otherValue);
        else if (!hasValue && !reading)
            throw JSON_PropertyException(propertyName.c_str(), "No property or adjustment could be found by this name");
    }

    template<typename T>
    void ReadFromArray(T& out, int index)
    {
        T otherValue;
        bool hasValue = GetValueFromArray(sourceArray, out, index);
        bool hasOtherValue = GetValueFromArray(otherSourceArray, otherValue, index);

        if (hasOtherValue)
        {
            Adjust(out, otherValue);
        }
        else if (!hasValue && !reading)
        {
            std::stringstream sstream;
            sstream << index;
            throw JSON_PropertyException(sstream.str().c_str(), "No property or adjustment could be found at this index");
        }
    }

    void ObjectErrorCheck(std::string propertyName);
    void ArrayErrorCheck(int index);

    template<typename T>
    void Adjust(T& a, T& b)
    {
        a += b;
    }
};

template<> void JSON_PropertyReader::Adjust(json_spirit::mArray& a, json_spirit::mArray& b);
template<> void JSON_PropertyReader::Adjust(json_spirit::mObject& a, json_spirit::mObject& b);
template<> void JSON_PropertyReader::Adjust(json_spirit::mValue& a, json_spirit::mValue& b);
