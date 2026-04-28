#include "stringlib.h"
#include <cstring>
#include <errno.h>


String::String() : m_data(nullptr), m_size(0) {}

String::String(const char* s)
{
    if (s) {
        m_size = std::strlen(s);
        allocate(m_size + 1);
        std::memcpy(m_data.get(), s, m_size);
        m_data[m_size] = '\0';
    } else {
        m_size = 0;
        m_data = nullptr;
    }
}

String::String(const String& other) : m_size(other.m_size)
{
    if (m_size > 0) {
        allocate(m_size + 1);
        std::memcpy(m_data.get(), other.m_data.get(), m_size + 1);
    } else {
        m_data = nullptr;
    }
}

String::String(String&& other) noexcept : m_data(std::move(other.m_data)), m_size(other.m_size)
{
    other.m_size = 0;
}

String::~String() = default;

String& String::operator=(const String& other)
{
    if (this != &other) {
        m_size = other.m_size;
        if (m_size > 0) {
            allocate(m_size + 1);
            std::memcpy(m_data.get(), other.m_data.get(), m_size + 1);
        } else {
            m_data.reset();
        }
    }
    return *this;
}

String& String::operator=(String&& other) noexcept
{
    if (this != &other) {
        m_data = std::move(other.m_data);
        m_size = other.m_size;
        other.m_size = 0;
    }
    return *this;
}

String& String::operator=(const char* s)
{
    if (s) {
        m_size = std::strlen(s);
        allocate(m_size + 1);
        std::memcpy(m_data.get(), s, m_size + 1);
    } else {
        m_size = 0;
        m_data.reset();
    }
    return *this;
}

const char* String::c_str() const noexcept
{
    return m_data ? m_data.get() : "";
}

char* String::data() noexcept
{
    return m_data.get();
}

size_t String::size() const noexcept { return m_size; }
size_t String::length() const noexcept { return m_size; }
bool String::empty() const noexcept { return m_size == 0; }

String::iterator String::begin() noexcept { return m_data.get(); }
String::const_iterator String::begin() const noexcept { return m_data.get(); }
String::iterator String::end() noexcept { return m_data.get() + m_size; }
String::const_iterator String::end() const noexcept { return m_data.get() + m_size; }

void String::allocate(size_t n)
{
    m_data = std::make_unique<char[]>(n);
}

// ---- Реализация C-функций (некоторые являются статическими по сути) ----
void* String::memcpy(void* dest, const void* src, size_t n)
{
    return std::memcpy(dest, src, n);
}

void* String::memmove(void* dest, const void* src, size_t n)
{
    return std::memmove(dest, src, n);
}

char* String::strcpy(char* dest, const char* src)
{
    return std::strcpy(dest, src);
}

char* String::strncpy(char* dest, const char* src, size_t n)
{
    return std::strncpy(dest, src, n);
}

char* String::strcat(char* dest, const char* src)
{
    return std::strcat(dest, src);
}

char* String::strncat(char* dest, const char* src, size_t n)
{
    return std::strncat(dest, src, n);
}

int String::memcmp(const void* s1, const void* s2, size_t n)
{
    return std::memcmp(s1, s2, n);
}

int String::strcmp(const char* s1, const char* s2)
{
    return std::strcmp(s1, s2);
}

int String::strcoll(const char* s1, const char* s2)
{
    return std::strcoll(s1, s2);
}

int String::strncmp(const char* s1, const char* s2, size_t n)
{
    return std::strncmp(s1, s2, n);
}

size_t String::strxfrm(char* dest, const char* src, size_t n)
{
    return std::strxfrm(dest, src, n);
}

char* String::strtok(char* str, const char* delim)
{
    static thread_local char* last = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    return ::strtok_s(str, delim, &last);
#else
    return ::strtok_r(str, delim, &last);
#endif
}

void* String::memset(void* s, int c, size_t n)
{
    return std::memset(s, c, n);
}

char* String::strerror(int errnum)
{
    // Используем thread-safe версию
    static thread_local char buf[256];
#ifdef _WIN32
    strerror_s(buf, sizeof(buf), errnum);
    return buf;
#else
    return strerror_r(errnum, buf, sizeof(buf));
#endif
}

size_t String::strlen(const char* s)
{
    return std::strlen(s);
}