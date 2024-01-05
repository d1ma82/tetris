#include <cstdio>


#ifdef ANDROID
    #include <android/log.h>

    #define TAG "anpr: "
    #define LOGI(...) (__android_log_print(ANDROID_LOG_INFO, TAG __FILE__, __VA_ARGS__));
    #define LOGE(...) (__android_log_print(ANDROID_LOG_ERROR, TAG __FILE__, __VA_ARGS__));
#else
    #define LOGI(str, ...) (printf( "%s: " str "\n", __FILE__, ##__VA_ARGS__));
    #define LOGE(format, ...)  	\
    {								\
    	fprintf(stderr, "[ERR] [%s:%s:%d] " format "\n", \
    	__FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
    }
    #ifdef NDEBUG//DEBUG_INFO   
        #define LOGD(str, ...) ;    
    #else   
        #define LOGD(str, ...) (printf( "%s: " str "\n", __FILE__, ##__VA_ARGS__));                                                           
    #endif             
#endif

