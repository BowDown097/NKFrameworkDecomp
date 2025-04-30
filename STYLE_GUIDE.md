# Style Guide

### Important Note
Always follow the rules, unless if it can be verified that something is being done a different way through symbols or error messages, because sometimes the devs just decide to not be consistent.

## Syntax
- The opening bracket of a code block is placed on the same line.
- Brackets are never omitted, even if the compiler allows doing so.
- Code is generally ensured to fit entirely on a conventional screen width (shoot for within 800px), meaning, in case of an overflow:
    - String literals are broken where appropriate (newline, end of a sentence, etc.). Do NOT use the + operator or break within the literal! Always break your string literals ``"like " "so"``.
    - Function parameters are put onto their own lines and fit as symetrically as possible (when this is done, the first line of the containing code block is typically also blank for readability).
    - Template parameters are put on their own lines, one-by-one.
- A type alias is typically made for type definitions which would certainly result in the rule above being broken when used. Type aliases do appear in function name symbols and error messages, so please check against those before creating one. If you do not see one being used, don't create one!
- Pointer and reference modifiers are adjacent to the type (``int*``, ``int&``).
- West const is used (``const int``, NOT ``int const``).
- #include's are separated by library (i.e. framework headers are separate from STL headers, which are separate from Boost headers) and are also separate from the header guard and, in a source file, its respective header file.

## Variables
- Variable names are in camel case and are prefixed to indicate some types and qualifiers.
- The known type prefixes are as follows:
    - booleans: bVariable
    - enums: eVariable
    - floating point values: fVariable
    - pointers: pVariable
    - strings: sVariable
    - unsigned integers (of any bitness): uVariable
- These type prefixes do rarely appear, but should not be used unless their usage is 100% known (e.g. exposed by a symbol):
    - maps: mapVariable
    - sets: setVariable
    - signed integers (of any bitness): iVariable
    - vectors (moreso if there's some kind of ambiguity): vecVariable
- The known qualifier prefixes are as follows:
    - global: g\_variable
    - non-public: m\_variable OR \_variable
    - public static: s\_variable
- Prefixes can be mixed.
- Static strings will typically only bear the string prefix and not the static prefix.
- Type prefixes (or a group of them) count as a "word" themselves, so for example, a string variable for a button name will be named sButtonName, not sbuttonName.
- Member variables should have their offsets **as it is in the binary** noted in a comment, in hex, padded to 2 characters, unless if they are the only member variable of a type.
- Member variable names can be inferred. If a name absolutely cannot be determined, then it will be in the format ``field_XX``, where XX is the offset as mentioned before, with no prefixes and not padded.

## Types
- Type names are in pascal case and are typically prefixed with the first letter of their kind as follows:
    - CMyClass
    - IMyInterface
    - SMyStruct
    - eMyEnum
- Type aliases are also in pascal case and are typically named one of these two ways, with the first being the most common:
    - tMyTypeAlias
    - MyTypeAlias\_t
- Enum members are put on their own lines, one-by-one, and are typically pascal case but are sometimes in screaming snake case.
- Enum classes are always preferred over plain enums.
- Template type parameters always use ``typename`` and almost always follow the T, U, V... pattern.

## Functions
- Function names are in pascal case and do not bear prefixes or suffixes, unless if differentiating a public and non-public implementation with the same name (e.g. Send and \_Send).
- Function parameters are in camel case and are prefixed like standard variables.
- Function parameters are named both in header files and source files.
- Function definitions should be kept to source files where possible.

## Files
- Source and header file names are in pascal case and are only prefixed if they contain one interface type that is reimplemented with the same name elsewhere (e.g. IFile, CFile -> IFile.h, File.h).
- Source files use the .cpp file extension and header files the .h file extension.
- The existence of a source file can usually (but not always!) be verified by searching "\_cpp" in the binary's function names in a disassembler, granted function name symbols are available.

<br><br><br><br><br>

Here's some code demonstrating pretty much everything:

```cpp
// Class.h
#pragma once

#include "Uncategorized/types.h"

#include <string>
#include <unordered_map>
#include <vector>

class IInterface {
public:
    using tChar16StringMap = std::unordered_map<
        std::basic_string<char16_t>,
        std::basic_string<char16_t>>;

    virtual void DoThing() const;
    virtual ~IInterface() = default;
private:
    template<typename T, typename U>
    tChar16StringMap VeryLongFunction(
        const std::string& a, const std::string& b, const std::string& c,
        const tChar16StringMap& d, const T& e, const U& f) {

        // ...
        return {};
    }
};

class CClass {
public:
    struct SStruct {
        int someInteger;
        unsigned uSomeUnsigned;
        std::string sSomeString;
    };

    enum class eErrorType {
        Fatal,
        NON_FATAL_WARN,
        Minor
    };

    bool CheckData(void* pData);
private:
    IInterface* m_pInterface; // 0x00
    eErrorType m_eResultError; // 0x08
    bool field_C; // 0x0C

    bool HandleError(const std::string& error, ushort code);
};



// Class.cpp
#include "Class.h"

#include "NewFramework/Utilities/StringHelper.h"

#include <iostream>

bool CClass::checkData(void* pData) {
    static std::string sNullDataError = "Oh no! This is very very bad! It's over! "
                                        "pData is null!";
    static ushort s_uNullDataErrorCode = 57;

    if (!pData) {
        return HandleError(sNullDataError, s_uNullDataErrorCode);
    }

    // ...

    return true;
}

bool CClass::HandleError(const std::string& error, ushort code) {
    std::cout << StringHelper::Format("ERROR %s (code: %hu)", error.c_str(), code) << std::endl;
    // ...
    return false;
}
```
