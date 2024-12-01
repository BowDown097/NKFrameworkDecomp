#include "JSON_Read.h"

void JSON_PropertyReader::Unload()
{
    delete sourceArray;
    delete otherSourceArray;
    delete sourceObject;
    delete otherSourceObject;
}

void JSON_PropertyReader::ObjectErrorCheck(std::string propertyName)
{
    if (sourceObject || otherSourceObject)
        return;

    if (sourceArray)
        throw JSON_PropertyException(propertyName.c_str(), "Object property read was called, but an array is loaded");
    else
        throw JSON_PropertyException(propertyName.c_str(), "No source object has been loaded");
}

void JSON_PropertyReader::ArrayErrorCheck(int index)
{
    std::stringstream sstream;
    sstream << index;

    if (!sourceArray && !otherSourceArray)
    {
        if (!sourceObject)
            throw JSON_PropertyException(sstream.str().c_str(), "No source array has been loaded.");
        else
            throw JSON_PropertyException(sstream.str().c_str(), "Array property read was called, but an object is loaded");
    }

    if (index < 0 || (sourceArray ? sourceArray->size() <= index : otherSourceArray->size() < index))
        throw JSON_PropertyException(sstream.str().c_str(), "Specified array index out of bounds");
}

// the implementation for Adjust is correct pretty sure, but...
// i have a feeling it's just inlining a TON of json_spirit's code,
// but i did a little look through and couldn't find anything that looks similar.
// but this explicit specialization stuff definitely doesn't look right.

template<> void JSON_PropertyReader::Adjust(json_spirit::mArray& a, json_spirit::mArray& b)
{
    for (const json_spirit::mValue& value : b)
        a.push_back(value);
}

template<> void JSON_PropertyReader::Adjust(json_spirit::mObject& a, json_spirit::mObject& b)
{
    for (auto it = b.begin(); it != b.end(); ++it)
    {
        auto itA = a.find(it->first);
        if (itA == a.end())
            a.emplace(it->first, it->second);
        else
            Adjust(itA->second, it->second);
    }
}

template<> void JSON_PropertyReader::Adjust(json_spirit::mValue& a, json_spirit::mValue& b)
{
    switch (a.type())
    {
    case json_spirit::Value_type::obj_type:
        Adjust(a.get_obj(), b.get_obj());
        break;
    case json_spirit::Value_type::array_type:
        Adjust(a.get_array(), b.get_array());
        break;
    case json_spirit::Value_type::str_type:
        a = a.get_str() + b.get_str();
        break;
    case json_spirit::Value_type::bool_type:
        a = b;
        break;
    case json_spirit::Value_type::int_type:
        a = a.get_int64() + b.get_int64();
        break;
    case json_spirit::Value_type::real_type:
        a = b.get_real() + a.get_real(); // yes, these are swapped, not sure why
        break;
    case json_spirit::Value_type::null_type:
        break;
    }
}
