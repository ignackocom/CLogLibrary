# CLog
Simple C Log system.<br/>
C89, C99 and >C99 compatible, CPP98 and >CPP98 compatible, Embedded C compatible.

Example of use

```c
#ifdef _MSC_VER
#pragma warning(disable:4464)
#endif /* _MSC_VER */

#include "../CLog/CLog.h"

#ifdef _MSC_VER
#pragma warning(default:4464)
#endif /* _MSC_VER */


#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif /* defined(__clang__) */


int main(int argc, char* argv[])
{
    const char* crlf = "\r\n";

    T_LOG_CHANNEL tchannel;
    
    int istatus;

    printf("Log version %ld\n", CLOG_VERSION());
    printf("\n");

//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_NONE); /* logging is off */
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_STDOUT("\r\n"));
    istatus = LOG_Init(&tchannel, LOG_CHANNEL_STDERR(crlf));
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_FILE("C:\\A\\", "Log", ".txt", "\n", LOG_PAGING_YEARS));
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_FILE("C:\\A\\", "Log", ".txt", "\n", LOG_PAGING_SECONDS));
    if (istatus == LOG_OK)
    {
        istatus = LOG_Trace(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Debug(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Info(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Warning(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Error(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Critical(&tchannel, "value %d %d", 10, 20);
        istatus = LOG_Fatal(&tchannel, "value %d %d", 10, 20);

        istatus = LOG_Close(&tchannel);
    }

    printf("\n");

    istatus = LOG_INIT(&tchannel, NULL, "stderr", NULL, crlf, LOG_PAGING_NONE);
    if (istatus == LOG_OK)
    {
        LOG_TRACE((&tchannel, "value %d %d", 40, 50));
        LOG_DEBUG((&tchannel, "value %d %d", 40, 50));
        LOG_INFO((&tchannel, "value %d %d", 40, 50));
        LOG_WARNING((&tchannel, "value %d %d", 40, 50));
        LOG_ERROR((&tchannel, "value %d %d", 40, 50));
        LOG_CRITICAL((&tchannel, "value %d %d", 40, 50));
        LOG_FATAL((&tchannel, "value %d %d", 40, 50));

        LOG_CLOSE(&tchannel);
    }

    return(0);
}
```

# History of changes ...
2025 05 15 better log functionalities with macros added<br/>
