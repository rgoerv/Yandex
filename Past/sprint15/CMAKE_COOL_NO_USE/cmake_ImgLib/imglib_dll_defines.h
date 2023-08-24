#pragma once

#if defined _WIN32 || defined __CYGWIN__ || defined __MINGW32__
    #ifdef IMGLIB_DLL
        #ifdef __GNUC__
            #define IMGLIB_EXPORT_DECL __attribute__ ((dllexport))
        #else
            #define IMGLIB_EXPORT_DECL __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define IMGLIB_EXPORT_DECL __attribute__ ((dllimport))
        #else
            #define IMGLIB_EXPORT_DECL __declspec(dllimport)
        #endif
    #endif
#else
    #define IMGLIB_EXPORT_DECL __attribute__ ((visibility ("default")))
#endif

#define IMGLIB_EXPORT extern "C" IMGLIB_EXPORT_DECL