/*--------------------------------------------------------------------------*/
// graph.cpp
// David Joffe 1998/12
// replacing the old djgega.cpp graphics interface
/*
Copyright (C) 1998-2001 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/
/*--------------------------------------------------------------------------*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL.h"
#include "sys_log.h"//Log
#include "djstring.h"//djStrPrintf

#include "graph.h"

// These dependencies suck
#include "djlog.h"

#include "djtypes.h"


djImage *g_pFont8x8=NULL;

djVisual *pVisMain = NULL;
djVisual *pVisBack = NULL;
djVisual *pVisView = NULL;

//Very simple pseudo 'console message' [dj2016-10]
std::string g_sMsg("");
int g_nConsoleMsgTimer=0;//[milliseconds]
void SetConsoleMessage( const std::string& sMsg )
{
	g_sMsg = sMsg;
	g_nConsoleMsgTimer = 2000;//Display message for ~2s then it disappears
}

// ScaleView should also be false for the level editor [why does that already work?]
void GraphFlip(bool bScaleView)
{
	const bool bShowFrameRate = false;//<- DEBUG? 'global' frame rate

	// in leveled i think pVisBack is NULL??? not sure [dj2016-10] don't want this in level editor
	SDL_Surface* pVisTemp = NULL;//<-cbsu/sbsu? create once not every frame? this is just to remember temporarily
	// what's 'behind' the console message and immediately restore it after, otherwise e.g. say subsequent
	// volume changes cause the console message to draw over its previous self in a sort of 'additive' way
	// leaving text looking messed up [dj2016-10]
	if (pVisBack!=NULL && g_pFont8x8!=NULL && (!g_sMsg.empty() || bShowFrameRate))
	{
		pVisTemp = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 8, pVisBack->pSurface->format->BitsPerPixel,
			pVisBack->pSurface->format->Rmask,
			pVisBack->pSurface->format->Gmask,
			pVisBack->pSurface->format->Bmask,
			0);
		if (pVisTemp)
		{
			SDL_Rect Rect;
			Rect.x=0;
			Rect.y=0;
			Rect.w=320;
			Rect.h=8;
			SDL_BlitSurface(pVisBack->pSurface, &Rect, pVisTemp, &Rect);
		}

		//SDL_GetTicks "Get the number of milliseconds since the SDL library initialization". Note that SDL_GetTicks wraps at ~49days
		static unsigned int uTimeLast = 0;
		if (uTimeLast==0)
			uTimeLast = SDL_GetTicks();
		unsigned int uTimeNow = SDL_GetTicks();

		if (!g_sMsg.empty())
		{
			GraphDrawString( pVisBack, g_pFont8x8, 0, 0, (const unsigned char*)g_sMsg.c_str() );
			
			// Clear console message after a certain time
			if (uTimeNow>=0 && uTimeNow>uTimeLast)
			{
				g_nConsoleMsgTimer -= ((int)uTimeNow - (int)uTimeLast);
				if (g_nConsoleMsgTimer<=0)
				{
					g_sMsg.clear();
					g_nConsoleMsgTimer = -1;
				}
			}
		}

		if (bShowFrameRate)
		{
			// Simple global frame rate display [?]
			// Be careful to avoid divide by zero here
			std::string sFrameRate;//Note this is 'instantaneous' frame rate i.e. last-frame-only so can look a bit jumpy, no smoothing
			if (uTimeNow>=0 && uTimeNow>uTimeLast)
			{
				sFrameRate = djStrPrintf("%.2f", 1000.f / (float)(uTimeNow - uTimeLast) );
				GraphDrawString( pVisBack, g_pFont8x8, 150, 0, (const unsigned char*)sFrameRate.c_str() );
			}
		}

		uTimeLast = uTimeNow;
	}

	djgFlip( pVisMain, pVisBack, bScaleView );

	// Restore to back buffer what was underneath the message
	if (pVisTemp)
	{
		SDL_Rect Rect;
		Rect.x=0;
		Rect.y=0;
		Rect.w=320;
		Rect.h=8;
		SDL_BlitSurface(pVisTemp, &Rect, pVisBack->pSurface, &Rect);

		// inefficient .. do this once not every frame ..
		SDL_FreeSurface(pVisTemp);
	}
}

bool GraphInit( bool bFullScreen, int iWidth, int iHeight )
{
	int imode;

	// Initialize graphics library
	SDL_Init(SDL_INIT_VIDEO);

	// [dj2016-10] Get the user's monitor resolution, and find (basically) largest multiple of 320x200 that fits in
	// that size, to make for 'largest possible' gameplay window, that also scales from 320x200 'proportionally' nicely
	// (i.e. square aspect ratio of pixels, etc.)
	// [low/future] - if 2 monitors, will this behave 'correct'
	const SDL_VideoInfo* vidinfo = SDL_GetVideoInfo();
	if (vidinfo)
	{
		// THIS MUST BE TESTED ON LINUX [dj2016-10]
		int max_w = vidinfo->current_w;
		int max_h = vidinfo->current_h;
		if (max_w>iWidth && max_h>iHeight)
		{
			int nMultiple = djMAX(1, djMIN( vidinfo->current_w / iWidth, vidinfo->current_h / iHeight ) );
			iWidth *= nMultiple;
			iHeight *= nMultiple;
		}

		Log( "DaveStartup(): DisplayResolution(%d,%d).\n", max_w, max_h );
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// Window dressing crap
	SDL_WM_SetCaption("Dave Gnukem", NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("data/icon.bmp"), NULL);
	// Hide mouse cursor
	SDL_ShowCursor(0);

	//--- (1) - Front buffer
	Log( "DaveStartup(): djgOpenVisual(w,h=%d,%d).\n", iWidth, iHeight );
	if (NULL == (pVisMain = djgOpenVisual( bFullScreen?"fullscreen":NULL, iWidth, iHeight )))
	{
		printf( "GraphInit(): COULDNT OPEN GMAIN\n" );
		return false;
	}
	Log( "DaveStartup(): Display bytes per pixel %d\n", pVisMain->bpp) ;
	imode = pVisMain->bpp;

	// Set the 32<->16 pixel conversion atributes, so the
	// images would be displayed correctly with any pixel format
	SetPixelConversion ( pVisMain );

	//--- (2) - Back buffer
	if (NULL == (pVisBack = djgOpenVisual( "memory", iWidth, iHeight, imode )))
	{
		printf( "GraphInit(): COULDNT OPEN GBACK\n" );
		return false;
	}

	//--- (3) - View buffer
	if (NULL == (pVisView = djgOpenVisual( "memory", iWidth, iHeight, imode )))
	{
		printf( "GraphInit(): COULDNT OPEN GVIEW\n" );
		return false;
	}

	//--- (5) - Load 8x8 font bitmap (FIXME error check)
	if (NULL != (g_pFont8x8 = new djImage))
	{
		g_pFont8x8->Load( FILE_IMG_FONT );
		djCreateImageHWSurface( g_pFont8x8 );
	}

	return true;
}

void GraphDone()
{
	djDEL(g_pFont8x8);

	djgCloseVisual( pVisView );
	djgCloseVisual( pVisBack );
	djgCloseVisual( pVisMain );

	djDEL(pVisView);
	djDEL(pVisBack);
	djDEL(pVisMain);

	SDL_Quit();
}

// FIXME: , view_height?
void GraphFlipView( int iViewWidth, int iViewHeight )
{
	djgDrawVisual( pVisBack, pVisView, 16, 16, 16, 16, iViewWidth*16, iViewHeight*16 );
}

// FIXME: Currenetly assumes a 256-char 32x8 character 256x128 pixel alpha-mapped image
void GraphDrawString( djVisual *pVis, djImage *pImg, int x, int y, const unsigned char *szStr )
{
	// FIXME: bounds check properyl
	if (x<0 || y<0) return;

	// Draw each character in the string
	for ( unsigned int i=0; i<strlen((char*)szStr); ++i )
	{
		int iIndex = (int)szStr[i];
		djgDrawImageAlpha( pVis, pImg, 8*(iIndex%32), 8*(iIndex/32), x+i*8, y, 8, 8 );
	}
}

