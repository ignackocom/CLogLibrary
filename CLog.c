/******************************************************************************
* \file      CLog.c
* \author    Peter Potrok
* \copyright Copyright (c) 1994 - 2025
*            MIT License (see License.txt file)
* \brief     C Log library
*            C89, C99 and >C99 compatible, CPP compatible, Embedded C compatible
* \details
*
* \see
*            and other resources
******************************************************************************/

#include "CLog.h"


#if defined(__clang__)
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif /* defined(__clang__) */


int LOG_Init(T_LOG_CHANNEL* ptchannel, char* pchpath, char* pchfilename, char* pchext, char* pchlinespace, int ipaging)
{
    /* channel clear */
    memset(ptchannel->chaPath, 0, sizeof(ptchannel->chaPath));
    memset(ptchannel->chaFilename, 0, sizeof(ptchannel->chaFilename));
    memset(ptchannel->chaExt, 0, sizeof(ptchannel->chaExt));
    memset(ptchannel->chaLineSpace, 0, sizeof(ptchannel->chaLineSpace));
    ptchannel->iPaging = LOG_PAGING_NONE;

    memset(ptchannel->chaPage, 0, sizeof(ptchannel->chaPage));
    
    ptchannel->fpFile = NULL;

    /* parameters check */
    if (pchpath != NULL && strlen(pchpath) > LOG_LEN_PATH)
        return(LOG_ERROR_PATH_TOO_LONG);
    if (pchfilename != NULL && strlen(pchfilename) > LOG_LEN_FILENAME)
        return(LOG_ERROR_FILENAME_TOO_LONG);
    if (pchext != NULL && strlen(pchext) > LOG_LEN_EXTENSION)
        return(LOG_ERROR_EXTENSION_TOO_LONG);
    if (pchlinespace != NULL && strlen(pchlinespace) > LOG_LEN_LINESPACE)
        return(LOG_ERROR_LINE_SPACE_TOO_LONG);
    if (ipaging < LOG_PAGING_NONE || ipaging > LOG_PAGING_SECONDS)
        return(LOG_ERROR_PAGING_OUT_OF_RANGE);

    /* channel set */
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif /* _MSC_VER */
    if (pchpath != NULL)
        strncpy(ptchannel->chaPath, pchpath, LOG_LEN_PATH);
    if (pchfilename != NULL)
        strncpy(ptchannel->chaFilename, pchfilename, LOG_LEN_FILENAME);
    if (pchext != NULL)
        strncpy(ptchannel->chaExt, pchext, LOG_LEN_EXTENSION);
    if (pchlinespace != NULL)
        strncpy(ptchannel->chaLineSpace, pchlinespace, LOG_LEN_LINESPACE);
#ifdef _MSC_VER
#pragma warning(default:4996)
#endif /* _MSC_VER */
    ptchannel->iPaging = ipaging;
    
    return(LOG_OK);
}


int log_message(T_LOG_CHANNEL* ptchannel, const char* const pchmessage, const char* const pchfmt, va_list args);

int log_message(T_LOG_CHANNEL* ptchannel, const char* const pchmessage, const char* const pchfmt, va_list args)
{
    time_t t;
    struct tm* ptm;

    char lchaPage[LOG_LEN_PAGE + 1];
    char lchaFullPath[LOG_LEN_PATH + LOG_LEN_FILENAME + LOG_LEN_PAGE + LOG_LEN_EXTENSION + 1];

    int len;
    int istatus;

    /* if channel NONE, return OK */
    if (strlen(ptchannel->chaPath) == 0 && strlen(ptchannel->chaFilename) == 0 && strlen(ptchannel->chaExt) == 0)
    {
        return(LOG_OK);
    }

    /* get log time */
    t = time(NULL);
    if (t == -1) return(LOG_ERROR_TIME_GET);

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif /* _MSC_VER */
    ptm = localtime(&t);
    if (ptm == NULL) return(LOG_ERROR_TIME_GET);
#ifdef _MSC_VER
#pragma warning(default:4996)
#endif /* _MSC_VER */

    /* if channel stderr, file ptr stderr */
    if (strcmp(ptchannel->chaFilename, "stderr") == 0)
    {
        ptchannel->fpFile = stderr;
    }
    /* channel stdout, file ptr stdout */
    else if (strcmp(ptchannel->chaFilename, "stdout") == 0)
    {
        ptchannel->fpFile = stdout;
    }
    else
    {
#if defined(__clang__)
#pragma clang unsafe_buffer_usage begin
#endif /* defined(__clang__) */

        /* if paging is set, set new page */
        if (ptchannel->iPaging != LOG_PAGING_NONE)
        {
            /* prepare new page string */
            memset(lchaPage, 0, sizeof(lchaPage));
            if (ptchannel->iPaging >= LOG_PAGING_YEARS)
                sprintf(&lchaPage[strlen(lchaPage)], "_%04d", ptm->tm_year + 1900);
            if (ptchannel->iPaging >= LOG_PAGING_MONTHS)
                sprintf(&lchaPage[strlen(lchaPage)], "_%02d", ptm->tm_mon + 1);
            if (ptchannel->iPaging >= LOG_PAGING_DAYS)
                sprintf(&lchaPage[strlen(lchaPage)], "_%02d", ptm->tm_mday);
            if (ptchannel->iPaging >= LOG_PAGING_HOURS)
                sprintf(&lchaPage[strlen(lchaPage)], "_%02d", ptm->tm_hour);
            if (ptchannel->iPaging >= LOG_PAGING_MINUTES)
                sprintf(&lchaPage[strlen(lchaPage)], "_%02d", ptm->tm_min);
            if (ptchannel->iPaging >= LOG_PAGING_SECONDS)
                sprintf(&lchaPage[strlen(lchaPage)], "_%02d", ptm->tm_sec);

            if (strcmp(ptchannel->chaPage, lchaPage) != 0)
            {
                memcpy(ptchannel->chaPage, lchaPage, sizeof(ptchannel->chaPage));

                /* close file */
                if (ptchannel->fpFile != NULL)
                {
                    istatus = fclose(ptchannel->fpFile);
                    if (istatus != 0) return(LOG_ERROR_FILE_CLOSE);
                    ptchannel->fpFile = NULL;
                }
            }
        }

        /* open file */
        if (ptchannel->fpFile == NULL)
        {
            /* create new full path */
            sprintf(lchaFullPath, "%s%s%s%s", ptchannel->chaPath, ptchannel->chaFilename, ptchannel->chaPage, ptchannel->chaExt);

            /* open file */
            ptchannel->fpFile = fopen(lchaFullPath, "a");
            if (ptchannel->fpFile == NULL) return(LOG_ERROR_FILE_OPEN);
        }

#if defined(__clang__)
#pragma clang unsafe_buffer_usage end
#endif /* defined(__clang__) */

    }

    len = 0;

    /* log date stamp */
    istatus = fprintf(ptchannel->fpFile, "%04d-%02d-%02d ", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
    if (istatus < 0) return(LOG_ERROR_FILE_WRITE);
    len += istatus;

    /* log time stamp */
    istatus = fprintf(ptchannel->fpFile, "%02d:%02d:%02d ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    if (istatus < 0) return(LOG_ERROR_FILE_WRITE);
    len += istatus;

    /* log message type */
    istatus = fprintf(ptchannel->fpFile, "%s", pchmessage);
    if (istatus < 0) return(LOG_ERROR_FILE_WRITE);
    len += istatus;

    /* log message */
    istatus = vfprintf(ptchannel->fpFile, (const char* const)pchfmt, args);
    if (istatus < 0) return(LOG_ERROR_FILE_WRITE);
    len += istatus;

    /* line spacing */
    istatus = fprintf(ptchannel->fpFile, "%s", ptchannel->chaLineSpace);
    if (istatus < 0) return(LOG_ERROR_FILE_WRITE);
    len += istatus;

    return(len);
}


int LOG_Trace(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|    TRACE | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Debug(T_LOG_CHANNEL* ptchannel, const char* const  pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|    DEBUG | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Info(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|     INFO | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Warning(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|  WARNING | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Error(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|    ERROR | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Critical(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "| CRITICAL | ", pchformat, args);
    va_end(args);

    return(istatus);
}

int LOG_Fatal(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...)
{
    va_list args;

    int istatus;

    va_start(args, pchformat);
    istatus = log_message(ptchannel, "|    FATAL | ", pchformat, args);
    va_end(args);

    return(istatus);
}


int LOG_Close(T_LOG_CHANNEL* ptchannel)
{
    int istatus;

    /* channel none */
    if (ptchannel->fpFile == NULL) return(LOG_OK);

    /* channel stderr */
    if (ptchannel->fpFile == stderr)
    {
        ptchannel->fpFile = NULL;
        return(LOG_OK);
    }

    /* channel stdout */
    if (ptchannel->fpFile == stdout)
    {
        ptchannel->fpFile = NULL;
        return(LOG_OK);
    }

    /* channel file */
    istatus = fclose(ptchannel->fpFile);
    if (istatus != 0) return(LOG_ERROR_FILE_CLOSE);
    
    ptchannel->fpFile = NULL;

    return(LOG_OK);
}
