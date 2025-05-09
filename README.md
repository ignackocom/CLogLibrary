# CLogLibrary
Simple C Log Library system.<br/>
C89, C99 and >C99 compatible, CPP98 and >CPP98 compatible, Embedded C compatible.

Example of use

```c
#include "../CLog/CLog.h"


int main(int argc, char* argv[])
{
    T_LOG_CHANNEL tchannel;
    
    int istatus;

//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_NONE); /* logging is off */
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_STDOUT("\r\n"));
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_STDERR("\r\n"));
//    istatus = LOG_Init(&tchannel, LOG_CHANNEL_FILE("C:\\A\\", "Log", ".txt", "\n", LOG_PAGING_YEARS));
    istatus = LOG_Init(&tchannel, LOG_CHANNEL_FILE("C:\\A\\", "Log", ".txt", "\n", LOG_PAGING_SECONDS));
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

    return(0);
}
```

# History of changes ...
2025 05 09 repository renamed to CLogLibrary
