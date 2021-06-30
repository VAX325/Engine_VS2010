#pragma once
#ifndef MACRO_H
#define MACRO_H

#if IS_WIN32
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif IS_UNIX || IS_MAC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT
#define IMPORT
#pragma warning (Unknown dynamic link import/export semantics.)
#endif

#if QRENDER9_COMPILING
#   define QRENDER9_PUBLIC EXPORT
#else
#   define QRENDER9_PUBLIC IMPORT
#endif

#if QRENDERGL_COMPILING
#   define QRENDERGL_PUBLIC EXPORT
#else
#   define QRENDERGL_PUBLIC IMPORT
#endif

#if QRENDER_COMPILING
#   define QRENDER_PUBLIC EXPORT
#else
#   define QRENDER_PUBLIC IMPORT
#endif

#if QENGINE_COMPILING
#   define QENGINE_PUBLIC EXPORT
#else
#   define QENGINE_PUBLIC IMPORT
#endif

#if QCORE_COMPILING
#   define QCORE_PUBLIC EXPORT
#else
#   define QCORE_PUBLIC IMPORT
#endif

#endif