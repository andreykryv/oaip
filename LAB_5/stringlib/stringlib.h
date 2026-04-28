#ifndef STRINGLIB_H
#define STRINGLIB_H

#include "stringlib_global.h"
#include <memory>
#include <cstring>
#include <stdexcept>

class STRINGLIB_EXPORT String
{
public:
    // Итератор (простой указатель, аналог std::string::iterator)
    using iterator = char*;
    using const_iterator = const char*;

    // Конструкторы
    String();
    explicit String(const char* s);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    // Операторы
    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String& operator=(const char* s);

    // Доступ к данным
    const char* c_str() const noexcept;
    char* data() noexcept;
    size_t size() const noexcept;
    size_t length() const noexcept;
    bool empty() const noexcept;

    // Итераторы
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    // C-функции
    void* memcpy(void* dest, const void* src, size_t n);
    void* memmove(void* dest, const void* src, size_t n);
    char* strcpy(char* dest, const char* src);
    char* strncpy(char* dest, const char* src, size_t n);
    char* strcat(char* dest, const char* src);
    char* strncat(char* dest, const char* src, size_t n);
    int memcmp(const void* s1, const void* s2, size_t n);
    int strcmp(const char* s1, const char* s2);
    int strcoll(const char* s1, const char* s2);
    int strncmp(const char* s1, const char* s2, size_t n);
    size_t strxfrm(char* dest, const char* src, size_t n);
    char* strtok(char* str, const char* delim);
    void* memset(void* s, int c, size_t n);
    char* strerror(int errnum);
    size_t strlen(const char* s);

private:
    std::unique_ptr<char[]> m_data;
    size_t m_size;
    void allocate(size_t n);
};

#endif // STRINGLIB_H