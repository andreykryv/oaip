#ifndef STRINGLIB_GLOBAL_H
#define STRINGLIB_GLOBAL_H

#if defined(_WIN32) || defined(_WIN64)
#ifdef STRINGLIB_LIBRARY
#define STRINGLIB_EXPORT __declspec(dllexport)
#else
#define STRINGLIB_EXPORT __declspec(dllimport)
#endif
#else
#define STRINGLIB_EXPORT __attribute__((visibility("default")))
#endif

#endif // STRINGLIB_GLOBAL_H