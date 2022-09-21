/////////////////////////////////////////////////////////////////////////////
// Purpose:   ttlib namespace functions and declarations
// Author:    Ralph Walden
// Copyright: Copyright (c) 1998-2022 KeyWorks Software (Ralph Walden)
// License:   Apache License -- see ../../LICENSE
/////////////////////////////////////////////////////////////////////////////

#pragma once

#if defined(_TTLIB_NAMESPACE_H_GUARD_)
    #error "You cannot #include both ttlib_wx.h and ttlibspace.h!"
#else
    #define _TTLIB_NAMESPACE_H_GUARD_  // sanity check to confirm that #pragma once is working as expected
#endif

// This is so that callers can conditionalize their code based on whether the header has been
// included.
#define _TTLIB_WX_HEADER_

#if !(__cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L))
    #error "The contents of <ttlib_wx.h> are available only with C++17 or later."
#endif

#include <cctype>
#include <filesystem>  // directory_entry
#include <stdlib.h>    // for std::abs(long)
#include <string>
#include <string_view>

#ifndef assertm
// assert with a message
    #define assertm(exp, msg) assert(((void) msg, exp))
#endif

// The tt namespace is for enums and constexpr values that only require a header. The ttlib namespace is for
// functions and classes that require linking to ttLib_wx.lib in order to use.

namespace tt
{
    /// Use to compare a size_t against -1
    constexpr size_t npos = static_cast<size_t>(-1);

    enum class CASE : size_t
    {
        exact,
        either,
        utf8  // comparisons are done by converting characters to lowercase UTF8
    };

    enum class TRIM : size_t
    {
        right,
        left,
        both,
        none
    };

    enum REPLACE : bool
    {
        once = false,
        all = true,
    };

}  // namespace tt

namespace ttlib
{
    class cstr;  // forward definition

    extern const std::string emptystring;

    // These functions are provided for convenience since they cast a char to unsigned char before calling the std::
    // library function.

    inline bool is_alnum(char ch) { return std::isalnum(static_cast<unsigned char>(ch)); }
    inline bool is_alpha(char ch) { return std::isalpha(static_cast<unsigned char>(ch)); }
    inline bool is_blank(char ch) { return std::isblank(static_cast<unsigned char>(ch)); }
    inline bool is_cntrl(char ch) { return std::iscntrl(static_cast<unsigned char>(ch)); }
    inline bool is_digit(char ch) { return std::isdigit(static_cast<unsigned char>(ch)); }
    inline bool is_graph(char ch) { return std::isgraph(static_cast<unsigned char>(ch)); }
    inline bool is_lower(char ch) { return std::islower(static_cast<unsigned char>(ch)); }
    inline bool is_print(char ch) { return std::isprint(static_cast<unsigned char>(ch)); }
    inline bool is_punctuation(char ch) { return std::ispunct(static_cast<unsigned char>(ch)); }
    inline bool is_upper(char ch) { return std::isupper(static_cast<unsigned char>(ch)); }
    inline bool is_whitespace(char ch) { return std::isspace(static_cast<unsigned char>(ch)); }

    // Is ch the start of a utf8 sequence?
    constexpr inline bool is_utf8(char ch) noexcept { return ((ch & 0xC0) != 0x80); }

    template <typename T>
    // Compares result against -1 -- use with returns from find, contains, locate, etc.
    constexpr bool is_found(T result)
    {
        return (static_cast<ptrdiff_t>(result)) != -1;
    }

    // Returns true if strings are identical
    bool is_sameas(std::string_view str1, std::string_view str2, tt::CASE checkcase = tt::CASE::exact);

    // Returns true if the sub-string is identical to the first part of the main string
    bool is_sameprefix(std::string_view strMain, std::string_view strSub, tt::CASE checkcase = tt::CASE::exact);

    // Return a view to the portion of the string beginning with the sub string.
    //
    // Return view is empty if substring is not found.
    std::string_view find_str(std::string_view main, std::string_view sub, tt::CASE checkcase = tt::CASE::exact);

    // Returns the position of sub within main, or npos if not found.
    size_t findstr_pos(std::string_view main, std::string_view sub, tt::CASE checkcase = tt::CASE::exact);

    // Returns true if the sub string exists withing the main string.
    //
    // Same as find_str but with a boolean return instead of a string_view.
    bool contains(std::string_view main, std::string_view sub, tt::CASE checkcase = tt::CASE::exact);

    template <class iterT>
    // Returns true if any string in the iteration list appears somewhere in the the main string.
    bool strContains(std::string_view str, iterT iter, tt::CASE checkcase = tt::CASE::exact)
    {
        for (auto& strIter: iter)
        {
            if (ttlib::contains(str, strIter, checkcase))
                return true;
        }
        return false;
    }

    // Combining has_member() and add_if() lets you use a std::vector like a std::set -- the vector will have have a lower
    // memory footprint, but searching will be slower.

    template <class T>
    bool has_member(const std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
    {
        for (size_t idx = 0; idx < vec.size(); ++idx)
        {
            if (ttlib::is_sameas(vec[idx], str, checkcase))
                return true;
        }
        return false;
    }

    template <class T>
#if defined(_WIN32)
    // Case-insensitive when compiled for Windows, otherwise case-sensitive
    bool has_filename(const std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::either)
#else
    // Case-insensitive when compiled for Windows, otherwise case-sensitive
    bool has_filename(const std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
#endif
    {
        for (size_t idx = 0; idx < vec.size(); ++idx)
        {
            if (ttlib::is_sameas(vec[idx], str, checkcase))
                return true;
        }
        return false;
    }

    template <class T>
    // Only adds the string if it doesn't already exist.
    void add_if(std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
    {
        for (size_t idx = 0; idx < vec.size(); ++idx)
        {
            if (ttlib::is_sameas(vec[idx], str, checkcase))
                return;
        }
        vec.emplace_back(str);
    }

    template <class T>
    size_t find_member(const std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
    {
        for (size_t idx = 0; idx < vec.size(); ++idx)
        {
            if (ttlib::is_sameas(vec[idx], str, checkcase))
                return idx;
        }
        return tt::npos;
    }

    // Search the entire vector for a member containing the substring.
    template <class T>
    size_t find_substr(std::vector<T>& vec, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
    {
        for (size_t idx = 0; idx < vec.size(); ++idx)
        {
            if (ttlib::contains(vec[idx], str))
                return idx;
        }
        return tt::npos;
    }

    // Search the vector from the given start index for a member containing the substring.
    template <class T>
    size_t find_substr(std::vector<T>& vec, size_t start, std::string_view str, tt::CASE checkcase = tt::CASE::exact)
    {
        for (size_t idx = start; idx < vec.size(); ++idx)
        {
            if (ttlib::contains(vec[idx], str))
                return idx;
        }
        return tt::npos;
    }

    // Returns a pointer to the next character in a UTF8 string.
    const char* next_utf8_char(const char* psz) noexcept;

    // Returns view to the next whitespace character. View is empty if there are no more
    // whitespaces.
    std::string_view find_space(std::string_view str) noexcept;

    // Returns position of next whitespace character or npos if not found.
    size_t find_space_pos(std::string_view str);

    // Returns view to the next non-whitespace character. View is empty if there are no
    // non-whitespace characters.
    std::string_view find_nonspace(std::string_view str) noexcept;

    // Returns position of next non-whitespace character or npos if not found.
    size_t find_nonspace_pos(std::string_view str) noexcept;

    // Equivalent to find_nonspace(find_space(str)).
    std::string_view stepover(std::string_view str) noexcept;

    // Equivalent to find_nonspace(find_space(str)) returning the position or npos.
    size_t stepover_pos(std::string_view str) noexcept;

    // Converts a string into an integer.
    //
    // If string begins with '0x' it is assumed to be hexadecimal and is converted.
    // String may begin with a '-' or '+' to indicate the sign of the integer.
    int atoi(std::string_view str) noexcept;

    // Converts a signed integer into a string.
    //
    // If format is true, the number will be formatted with ',' or '.' depending on the
    // current locale.
    ttlib::cstr itoa(int val, bool format = false);

    // Converts a size_t into a string.
    //
    // If format is true, the number will be formatted with ',' or '.' depending on the
    // current locale.
    ttlib::cstr itoa(size_t val, bool format = false);

    // Return a view to a filename's extension. View is empty if there is no extension.
    std::string_view find_extension(std::string_view str);

    // Determines whether the character at pos is part of a filename. This will
    // differentiate between '.' being used as part of a path (. for current directory, or ..
    // for relative directory) versus being the leading character in a file.
    bool is_valid_filechar(std::string_view str, size_t pos);

    // Converts all backslashes in a filename to forward slashes.
    //
    // Note: Windows handles paths that use forward slashes, so backslashes are normally
    // unnecessary.
    void backslashestoforward(std::string& str);

    // Performs a check to see if a directory entry is a filename and contains the
    // specified extension.
    bool has_extension(std::filesystem::directory_entry name, std::string_view extension,
                       tt::CASE checkcase = tt::CASE::exact);

    // Confirms newdir exists and is a directory and then changes to that directory.
    //
    // Returns false only if newdir is not an existing directory. Throws filesystem_error
    // if the directory is valid but could not be changed to.
    bool ChangeDir(std::string_view newdir);

    bool dir_exists(std::string_view dir);
    bool file_exists(std::string_view filename);

    void utf8to16(std::string_view str, std::wstring& dest);
    void utf16to8(std::wstring_view str, std::string& dest);

    std::wstring utf8to16(std::string_view str);
    std::string utf16to8(std::wstring_view str);

    // Remove locale-dependent whitespace from right side of string
    inline void RightTrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    }

    // Remove locale-dependent whitespace from left side of string
    inline void LeftTrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    }

    // Remove locale-dependent whitespace from left and right side of string
    inline void BothTrim(std::string& s)
    {
        ttlib::LeftTrim(s);
        ttlib::RightTrim(s);
    }

#if defined(_WX_DEFS_H_)

    // This should *ONLY* be called in the main/GUI thread.
    //
    // This function is available in Release builds, the ttASSERT macros only call it in
    // Debug builds
    bool AssertDialog(const char* filename, const char* function, int line, const char* cond, const std::string& msg);
#endif  // defined(_WX_DEFS_H_)

}  // namespace ttlib

// clang-format off

#if defined(_WIN32)

#include <windows.h>

// clang-format on

//////////////////////////////// Windows-only section ////////////////////////
//                                                                          //
// The following functions can only be used when compiling for Windows.     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

class wxString;

namespace ttlib
{
    // Converts all text to UTF16 before calling ShellExecuteW(...)
    HINSTANCE ShellRun(std::string_view filename, std::string_view args, std::string_view directory,
                       INT nShow = SW_SHOWNORMAL, HWND hwndParent = NULL);

    HINSTANCE ShellRun_wx(const wxString& filename, const wxString& args, const wxString& directory,
                          INT nShow = SW_SHOWNORMAL, HWND hwndParent = NULL);
}  // namespace ttlib

#endif  // end _WIN32 section

#if defined(_WX_DEFS_H_)
    // See assertion_dlg.h for an alternative to these macros
    #if defined(NDEBUG) && !defined(INTERNAL_TESTING)
        #define ttASSERT(cond)
        #define ttASSERT_MSG(cond, msg)
        #define ttFAIL_MSG(msg)
    #else  // not defined(NDEBUG) && !defined(INTERNAL_TESTING)
        #define ttASSERT(cond)                                                           \
            if (!(cond) && ttlib::AssertDialog(__FILE__, __func__, __LINE__, #cond, "")) \
            {                                                                            \
                wxTrap();                                                                \
            }

        #define ttASSERT_MSG(cond, msg)                                                     \
            if (!(cond) && ttlib::AssertDialog(__FILE__, __func__, __LINE__, #cond, (msg))) \
            {                                                                               \
                wxTrap();                                                                   \
            }

        #define ttFAIL_MSG(msg)                                                   \
            if (ttlib::AssertDialog(__FILE__, __func__, __LINE__, "failed", msg)) \
            {                                                                     \
                wxTrap();                                                         \
            }
    #endif  // defined(NDEBUG) && !defined(INTERNAL_TESTING)
#endif      // defined(_WX_DEFS_H_)
