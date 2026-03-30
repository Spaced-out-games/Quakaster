// Engine/lib.h
#pragma once
#ifdef QK_SHARED
#ifdef QK_EXPORT
#define QK_API __declspec(dllexport)
#else
#define QK_API __declspec(dllimport)
#endif
#else
#define QK_API  // static — no decoration needed
#endif




int QK_API foo(int, int);

