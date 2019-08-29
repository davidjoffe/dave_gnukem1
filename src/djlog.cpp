/*
djlog.cpp

Copyright (C) 1998-2019 David Joffe
*/

#ifdef WIN32
#include <windows.h>
#endif

#include "djlog.h"
#include <stdio.h>
#include <stdarg.h>//va_list etc.

void log_message( const char * szFormat, ... )
{
	static char buf[4096]={0};

	if ( szFormat == NULL )
		return;

	// print the formatted log string onto buf
	va_list args;
	va_start(args, szFormat);
	vsnprintf(buf, 4096, szFormat, args);
	va_end(args);

#ifdef WIN32
   // send message to debugger
   OutputDebugString( (LPCTSTR)buf );
#else
   // dump message on stdout
   printf( "%s", buf );
   fflush(NULL);
#endif
}

