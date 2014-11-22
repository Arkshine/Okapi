#ifndef _INCLUDE_OKAPI_VERSIONLIB_H_
#define _INCLUDE_OKAPI_VERSIONLIB_H_

#if !defined(OKAPI_USE_VERSIONLIB)
// These get defined in okapi_version.h since
// versionlib does not use versionlib.
    #undef OKAPI_LOCAL_REV
    #undef OKAPI_CSET
    #undef OKAPI_VERSION
    #undef OKAPI_BUILD_TIME
#endif

#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C extern
#endif

EXTERN_C const char *OKAPI_LOCAL_REV;
EXTERN_C const char *OKAPI_SHA;
EXTERN_C const char *OKAPI_VERSION;
EXTERN_C const char *OKAPI_BUILD_TIME;

#endif // _INCLUDE_OKAPI_VERSIONLIB_H_
