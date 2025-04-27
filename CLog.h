/******************************************************************************
* \file      CLog.h
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

#ifndef CLOG_H
#define CLOG_H      20250318L


/******************************************************************************
*  file paging structure
*
*          filename_year_month_day_hour_minute_second.extension
*
*  examples for windows
*
*  c:\path\filename.ext                       no paging
*  c:\path\filename_2025.ext                  paging by year
*  c:\path\filename_2025_04.ext               paging by month
*  c:\path\filename_2025_04_20.ext            paging by day
*  c:\path\filename_2025_04_20_12_07_01.ext   paging by second
*
*  examples for unix/linux
*
*  /path/filename.ext
*  /path/filename_2025.ext
*  /path/filename_2025_04.ext
*  /path/filename_2025_04_20.ext
*  /path/filename_2025_04_20_12_07_01.ext
*
*  log files view must be ordered according to modification date in OS
* 
*/


/* necessary includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>


/* maximum length of full file path parts */
#define LOG_LEN_PATH                                128 /* path terminated by a separator */
#define LOG_LEN_FILENAME                            16  /* filename without paging and extension */
#define LOG_LEN_EXTENSION                           4   /* filename extension with leading dot character */
#define LOG_LEN_LINESPACE                           2   /* line spacing */
#define LOG_LEN_PAGE                                20  /* paging part of the filename, automatically created by the library */

/* paging constants for file creation */
#define LOG_PAGING_NONE                             0
#define LOG_PAGING_YEARS                            1   /* paging by years */
#define LOG_PAGING_MONTHS                           3   /* paging by months */
#define LOG_PAGING_DAYS                             4   /* paging by days */
#define LOG_PAGING_HOURS                            5   /* paging by hours */
#define LOG_PAGING_MINUTES                          6   /* paging by minutes */
#define LOG_PAGING_SECONDS                          7   /* paging by seconds */

/* macros for creating channels */
#define LOG_CHANNEL_NONE                            NULL,NULL,NULL,NULL,LOG_PAGING_NONE /* logging is off */
#define LOG_CHANNEL_STDOUT(lspc)                    NULL,"stdout",NULL,(lspc),LOG_PAGING_NONE
#define LOG_CHANNEL_STDERR(lspc)                    NULL,"stderr",NULL,(lspc),LOG_PAGING_NONE
#define LOG_CHANNEL_FILE(path,file,ext,lspc,page)   (path),(file),(ext),(lspc),(page)

/* error code constants */
#define LOG_OK                                       0
#define LOG_ERROR_PATH_TOO_LONG                     -1
#define LOG_ERROR_FILENAME_TOO_LONG                 -2
#define LOG_ERROR_EXTENSION_TOO_LONG                -3
#define LOG_ERROR_LINE_SPACE_TOO_LONG               -4
#define LOG_ERROR_PAGING_OUT_OF_RANGE               -5
#define LOG_ERROR_TIME_GET                          -6
#define LOG_ERROR_FILE_OPEN                         -7
#define LOG_ERROR_FILE_WRITE                        -8
#define LOG_ERROR_FILE_CLOSE                        -9


/* channel structure */
typedef struct
{
    char  chaPath[LOG_LEN_PATH + 1];
    char  chaFilename[LOG_LEN_FILENAME + 1];
    char  chaExt[LOG_LEN_EXTENSION + 1];
    char  chaLineSpace[LOG_LEN_LINESPACE + 1];
    int   iPaging;

    char  chaPage[LOG_LEN_PAGE + 1];

    FILE* fpFile;
}T_LOG_CHANNEL;


#ifdef __cplusplus
extern "C" {
#endif

    /******************************************************************************
     * \brief
     *	The LOG_Init function initializes a logging channel by setting its parameters,
     *  checking for errors in the provided values, and preparing it for use.
     *
     * \param
     *	ptchannel - Channel structure pointer.
     * \param
     *	pchpath - Path.
     * \param
     *	pchfilename - Filename.
     * \param
     *	pchext - Extension with leading dot character.
     * \param
     *	pchlinespace - Line space characters.
     * \param
     *	ipaging - File paging.
     *
     * \return
     *	0 if success, otherwise error code.
     *
     */
    int LOG_Init(T_LOG_CHANNEL* ptchannel, char* pchpath, char* pchfilename, char* pchext, char* pchlinespace, int ipaging);


    /******************************************************************************
     * \brief
     *	The LOG_Trace function logs a trace message to a specified logging channel, 
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Trace(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Debug function logs a debug message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Debug(T_LOG_CHANNEL* ptchannel, const char* const  pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Info function logs a info message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Info(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Warning function logs a warning message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Warning(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Error function logs a error message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Error(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Critical function logs a critical message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Critical(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);

    /******************************************************************************
     * \brief
     *	The LOG_Fatal function logs a fatal message to a specified logging channel,
     *  formatting the message according to the provided format string and additional arguments.
     */
    int LOG_Fatal(T_LOG_CHANNEL* ptchannel, const char* const pchformat, ...);


    /******************************************************************************
     * \brief
     *	The LOG_Close function closes a logging channel represented by a T_LOG_CHANNEL pointer, 
     *  ensuring that any associated file stream is properly closed and the channel is reset.
     */
    int LOG_Close(T_LOG_CHANNEL* ptchannel);

#ifdef __cplusplus
}
#endif


#endif /* CLOG_H */
