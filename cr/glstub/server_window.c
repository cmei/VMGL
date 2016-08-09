/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#include "server.h"
#include "server_dispatch.h"
#include "cr_mem.h"
#include "cr_rand.h"


/**
 * Generate a new window ID.
 */
static GLint
generateID(void)
{
	if (cr_server.uniqueWindows) {
		static GLboolean firstCall = GL_TRUE;
		if (firstCall) {
			crRandAutoSeed();
			firstCall = GL_FALSE;
		}
		return crRandInt(20000, 50000); /* XXX FIX */
	}
	else {
		static GLint freeID = 1;
		return freeID++;
	}
}

void initGLStubWindow(GLint spuWindow, GLint* windowID, GLint dims[2])
{
    CRMuralInfo *mural;

    /*
     * Create a new mural for the new window.
     */
    mural = (CRMuralInfo *) crCalloc(sizeof(CRMuralInfo));
    CRASSERT(mural);
    if (mural) {
        CRMuralInfo *defaultMural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, 0);
        CRASSERT(defaultMural);
        mural->width = defaultMural->width;
        mural->height = defaultMural->height;
        mural->optimizeBucket = 0; /* might get enabled later */
        mural->numExtents = defaultMural->numExtents;
        mural->curExtent = 0;
        crMemcpy(mural->extents, defaultMural->extents,
                defaultMural->numExtents * sizeof(CRExtent));
        mural->underlyingDisplay[0] = 0;
        mural->underlyingDisplay[1] = 0;
        mural->underlyingDisplay[2] = dims[0];
        mural->underlyingDisplay[3] = dims[1];

        mural->spuWindow = spuWindow;
        glStubInitializeTiling(mural);

        // NOTE: previously, we would call generateID() for CR_TCPIP connection and do this only for CR_FILE.
        *windowID = spuWindow;

        crHashtableAdd(cr_server.muralTable, *windowID, mural);
    }

}

GLint SERVER_DISPATCH_APIENTRY
glStubDispatchWindowReuse( const char *dpyName, GLint visBits, GLint window )
{
	CRMuralInfo *mural;
	GLint windowID = -1;
	GLint spuWindow;
	GLint dims[2];

        /* This code won't play nice with state resumption... */
        CRASSERT(!cr_server.sharedWindows);

	/*
	 * Have first SPU make a new window.
	 */
	spuWindow = cr_server.head_spu->dispatch_table.WindowReuse( dpyName, visBits, window );
	if (spuWindow < 0) {
                crDebug("GLStub: client %p failed to reuse window (xwindow=0x%x, return=%d)",
                                                cr_server.curClient, window, spuWindow);
		glStubReturnValue( &spuWindow, sizeof(spuWindow) );
		return spuWindow;
	}

	/* get initial window size */
	cr_server.head_spu->dispatch_table.GetChromiumParametervCR(GL_WINDOW_SIZE_CR, spuWindow, GL_INT, 2, dims);

        initGLStubWindow(spuWindow, &windowID, dims);
        CRASSERT(spuWindow == windowID);

	crDebug("GLStub: client %p reused window %d (xwindow=0x%x, SPU window %d)",
					cr_server.curClient, windowID, window, spuWindow);

	glStubReturnValue( &windowID, sizeof(windowID) );
	return windowID;

}

GLint SERVER_DISPATCH_APIENTRY
glStubDispatchWindowCreate( const char *dpyName, GLint visBits )
{
	CRMuralInfo *mural;
	GLint windowID = -1;
	GLint spuWindow;
	GLint dims[2];

        /* This code won't play nice with state resumption... */
        CRASSERT(!cr_server.sharedWindows);

	/*
	 * Have first SPU make a new window.
	 */
	spuWindow = cr_server.head_spu->dispatch_table.WindowCreate( dpyName, visBits );
	if (spuWindow < 0) {
		glStubReturnValue( &spuWindow, sizeof(spuWindow) );
		return spuWindow;
	}

	/* get initial window size */
	cr_server.head_spu->dispatch_table.GetChromiumParametervCR(GL_WINDOW_SIZE_CR, spuWindow, GL_INT, 2, dims);

        initGLStubWindow(spuWindow, &windowID, dims);
        CRASSERT(spuWindow == windowID);

	crDebug("GLStub: client %p created new window %d (SPU window %d)",
					cr_server.curClient, windowID, spuWindow);

	glStubReturnValue( &windowID, sizeof(windowID) );
	return windowID;
}


#define EXTRA_WARN 0

void SERVER_DISPATCH_APIENTRY
glStubDispatchWindowDestroy( GLint window )
{
    CRMuralInfo *mural;
    GLint windowID = -1;
    GLint spuWindow;
    GLint dims[2];

    mural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, window);
    if (!mural) {
#if EXTRA_WARN
        crWarning("GLStub: invalid window %d passed to WindowDestroy()", window);
#endif
        return;
    }

    crHashtableDelete(cr_server.muralTable, window, NULL);
    crFree(mural);

    crDebug("GLStub: Destroying window %d (spu window %d)", window, mural->spuWindow);
    cr_server.head_spu->dispatch_table.WindowDestroy( mural->spuWindow );
}


void SERVER_DISPATCH_APIENTRY
glStubDispatchWindowSize( GLint window, GLint width, GLint height )
{
  CRMuralInfo *mural;

	/*	crDebug("GLStub: Window %d size %d x %d", window, width, height);*/
	mural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, window);
	if (!mural) {
#if EXTRA_WARN
		 crWarning("GLStub: invalid window %d passed to WindowSize()", window);
#endif
		 return;
	}
	mural->underlyingDisplay[2] = width;
	mural->underlyingDisplay[3] = height;
	glStubInitializeTiling(mural);

	cr_server.head_spu->dispatch_table.WindowSize(mural->spuWindow, width, height);
}


void SERVER_DISPATCH_APIENTRY
glStubDispatchWindowPosition( GLint window, GLint x, GLint y )
{
  CRMuralInfo *mural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, window);
	/*	crDebug("GLStub: Window %d pos %d, %d", window, x, y);*/
	if (!mural) {
#if EXTRA_WARN
		 crWarning("GLStub: invalid window %d passed to WindowPosition()", window);
#endif
		 return;
	}
#if EXTRA_WARN /* don't believe this is needed */
	mural->underlyingDisplay[0] = x;
	mural->underlyingDisplay[1] = y;
	glStubInitializeTiling(mural);
#endif
	cr_server.head_spu->dispatch_table.WindowPosition(mural->spuWindow, x, y);
}



void SERVER_DISPATCH_APIENTRY
glStubDispatchWindowShow( GLint window, GLint state )
{
  CRMuralInfo *mural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, window);
	if (!mural) {
#if EXTRA_WARN
		 crWarning("GLStub: invalid window %d passed to WindowShow()", window);
#endif
		 return;
	}
	cr_server.head_spu->dispatch_table.WindowShow(mural->spuWindow, state);
}


GLint
glStubSPUWindowID(GLint serverWindow)
{
  CRMuralInfo *mural = (CRMuralInfo *) crHashtableSearch(cr_server.muralTable, serverWindow);
	if (!mural) {
#if EXTRA_WARN
		 crWarning("GLStub: invalid window %d passed to glStubSPUWindowID()",
							 serverWindow);
#endif
		 return -1;
	}
	return mural->spuWindow;
}
