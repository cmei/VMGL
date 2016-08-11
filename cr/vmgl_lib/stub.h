/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */


/*
 * How this all works...
 *
 * This directory implements two different interfaces to Chromium:
 *
 * 1. the Chromium interface - this is defined by the functions that start
 *    with the "cr" prefix and are defined in chromium.h and implemented in
 *    stub.c.  Typically, this is used by parallel apps (like psubmit).
 *
 * 2. GLX emulation interface - the glX*() functions are emulated here.
 *    When glXCreateContext() is called we may either create a real, native
 *    GLX context or a Chromium context (depending on match_window_title and
 *    mimimum_window_size).
 *
 */


#ifndef CR_STUB_H
#define CR_STUB_H

#include "chromium.h"
#include "cr_hash.h"
#include "cr_process.h"
#include "cr_spu.h"
#include "cr_threads.h"
#include "spu_dispatch_table.h"


/* When we first create a rendering context we can't be sure whether
 * it'll be handled by Chromium or as a native GLX context.  So in
 * CreateContext() we'll mark the ContextInfo object as UNDECIDED then
 * switch it to either NATIVE or CHROMIUM the first time MakeCurrent()
 * is called.  In MakeCurrent() we can use a criteria like window size
 * or window title to decide between CHROMIUM and NATIVE.
 */
typedef enum
{
	UNDECIDED,
	CHROMIUM,
	NATIVE
} ContextType;

#define MAX_DPY_NAME 1000

typedef enum {
    VM_NATIVE,
    VM_LIVE_MIGRATION,
} VMMode;

typedef struct context_info_t ContextInfo;
typedef struct window_info_t WindowInfo;
typedef struct visual_info_t VisualInfo;
typedef struct display_info_t DisplayInfo;

struct context_info_t
{
	GLint spuContext;  /* returned by head SPU's CreateContext() */
	ContextType type;  /* CHROMIUM, NATIVE or UNDECIDED */
	unsigned long id;          /* the client-visible handle */
	GLint visBits;
	WindowInfo *currentDrawable;
#if defined(GLX)
	DisplayInfo *displayInfo;
	ContextInfo *share;
        VisualInfo *visualInfo;
	Bool direct;
	GLXContext nativeContext;
#endif
};
ContextInfo* trans_ContextInfo(GLXContext contextId, int returnNullOnFail);
void remove_ContextInfo(ContextInfo * contextInfo);

struct window_info_t
{
	char dpyName[MAX_DPY_NAME];
	int x, y;
	unsigned int width, height;
	ContextType type;
	GLint spuWindow;       /* returned by head SPU's WindowCreate() */
	GLboolean mapped;
#if defined(GLX)
        DisplayInfo * displayInfo;
	GLXDrawable drawabl;
        /* On resume from live migration, reset this to a new xwindow created from the new xserver.
         */
        Window nativeDrawable;
#endif
};

/**
 * Keep a list of structures which associates X visual IDs with
 * Chromium visual bitmasks.
 */
struct visual_info_t {
        // The value returned to the user of the API (may no longer be valid).
        XVisualInfo* visual;
        // The value currently active with the X server we're connected to (must be valid).
        // Gets reset when switching to VM_NATIVE or VM_LIVE_MIGRATION.
        XVisualInfo* currentVisual;
        // Information for reconstructing Visual's when we switch between VM_NATIVE and 
        // VM_LIVE_MIGRATION.
	int visBits;
        int * attribList;
        int attribListSize;
        VMMode vm_mode;
};
VisualInfo* trans_VisualInfo(XVisualInfo *vis);

struct display_info_t {
    /* dpy:        The value returned by XOpenDisplay(...) when it was first called.
     * currentDpy: The current value of XOpenDisplay(...) 
     *             (switches during VM_NATIVE <-> VM_LIVE_MIGRATION).
     */
    Display * dpy;
    Display * currentDpy;

    char display[64];
    char currentDisplay[64];
};
DisplayInfo* add_DisplayInfo(Display * ret, _Xconst char* display_name);
void remove_DisplayInfo(DisplayInfo * displayInfo);
DisplayInfo* trans_DisplayInfo(Display *dpy);

/* "Global" variables for the stub library */
typedef struct {
        DisplayInfo* currentDisplay;
        GLXDrawable currentDrawable;

	/* the first SPU in the SPU chain on this app node */
	SPU *spu;

	/* OpenGL/SPU dispatch tables */
	crOpenGLInterface wsInterface;
	SPUDispatchTable spuDispatch;
	SPUDispatchTable nativeDispatch;
	GLboolean haveNativeOpenGL;

	/* config options */
	int appDrawCursor;
	GLuint minChromiumWindowWidth;
	GLuint minChromiumWindowHeight;
	GLuint maxChromiumWindowWidth;
	GLuint maxChromiumWindowHeight;
	GLuint matchChromiumWindowCount;
	GLuint matchChromiumWindowCounter;
	GLuint *matchChromiumWindowID;
	GLuint numIgnoreWindowID;
	char *matchWindowTitle;
	int ignoreFreeglutMenus;
	int trackWindowSize;
	int trackWindowPos;
	int trackWindowVisibility;
	char *spu_dir;
	int force_pbuffers;

	/* thread safety stuff */
	GLboolean threadSafe;
#ifdef CHROMIUM_THREADSAFE
	CRtsd dispatchTSD;
	CRmutex mutex;
#endif

	/* contexts */
	int freeContextNumber;
        /* (user visible) context id (generated in add_ContextInfo, return by glXCreateContext) -> ContextInfo*
         */
	CRHashTable *contextTable;
	ContextInfo *currentContext; /* may be NULL */

	/* windows */
	CRHashTable *windowTable;

        /* 
         * VM migration 
         */

        /* original (user visible) Display* -> DisplayInfo* 
         */
	CRHashTable *displayTable;
        /* original (user visible) XVisualInfo* -> VisualInfo* 
         */
	CRHashTable *visualTable;

        VMMode vm_mode;
        SPU* render_spu;
} Stub;

extern Stub stub;
extern SPUDispatchTable glim;
extern SPUDispatchTable stubThreadsafeDispatch;
extern SPUDispatchTable stubNULLDispatch;


#if defined(GLX)

/* GLX versions */
extern WindowInfo *stubGetWindowInfo( GLXDrawable drawable );
extern void stubUseXFont( Display *dpy, Font font, int first, int count, int listbase );

#endif


extern void stubSetDispatch( SPUDispatchTable *table );
GLint GetVisBits(VisualInfo* visualInfo);
ContextInfo *
add_ContextInfo(DisplayInfo* displayInfo, 
        VisualInfo *visualInfo, 
        GLint visBits, ContextType type,
        Bool direct,
	GLXContext shareList);
extern void stubDestroyContext( unsigned long contextId );
extern GLboolean stubMakeCurrent( WindowInfo *window, ContextInfo *context );
extern GLint stubNewWindow( const char *dpyName, GLint visBits );
extern GLint stubReuseWindow( const char *dpyName, GLint visBits, GLint window );
extern void stubSwapBuffers( const WindowInfo *window, GLint flags );
extern void stubGetWindowGeometry( const WindowInfo *win, int *x, int *y, unsigned int *w, unsigned int *h );
extern GLboolean stubIsWindowVisible( const WindowInfo *win );
extern void stubInit(void);
extern int hasNativeDisplay(void);
extern Display * getNativeDisplay(void);

extern VisualInfo * FindVisualInfo(XVisualInfo* vis);

extern void APIENTRY stub_GetChromiumParametervCR( GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values );

extern void APIENTRY glBoundsInfoCR(const CRrecti *, const GLbyte *, GLint, GLint);

#endif /* CR_STUB_H */
