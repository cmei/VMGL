/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

/* As far as I can tell, these functions generally don't get called in the GL app, or in 
 * libvmgl.so.
 */

#include "cr_spu.h"
#include "cr_error.h" 
#include "cr_mem.h" 
#include "stub.h"

/**
 * Returns -1 on error
 */
GLint APIENTRY crCreateContext( const char *dpyName, GLint visBits )
{
        CRASSERT(0);
#if 0
	ContextInfo *context;
	stubInit();
	/* XXX in Chromium 1.5 and earlier, the last parameter was UNDECIDED.
	 * That didn't seem right so it was changed to CHROMIUM. (Brian)
	 */
	context = stubNewContext(dpyName, visBits, CHROMIUM, 0);
	return context ? (int) context->id : -1;
#endif
        return 0;
}

void APIENTRY crDestroyContext( GLint context )
{
        CRASSERT(0);
#if 0
  	stubDestroyContext(context);
#endif
        return;
}

void APIENTRY crMakeCurrent( GLint window, GLint context )
{
        CRASSERT(0);
#if 0
	WindowInfo *winInfo = (WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	ContextInfo *contextInfo = (ContextInfo *)
		crHashtableSearch(stub.contextTable, context);
	if (contextInfo && contextInfo->type == NATIVE) {
		crWarning("Can't call crMakeCurrent with native GL context");
		return;
	}

	stubMakeCurrent(winInfo, contextInfo);
#endif
        return;
}

GLint APIENTRY crGetCurrentContext( void )
{
        CRASSERT(0);
#if 0
	stubInit();
	if (stub.currentContext)
	  return (GLint) stub.currentContext->id;
	else
	  return 0;
#endif
        return 0;
}

GLint APIENTRY crGetCurrentWindow( void )
{
        CRASSERT(0);
#if 0
	stubInit();
	if (stub.currentContext && stub.currentContext->currentDrawable)
	  return stub.currentContext->currentDrawable->spuWindow;
	else
	  return -1;
#endif
        return 0;
}

void APIENTRY crSwapBuffers( GLint window, GLint flags )
{
        CRASSERT(0);
#if 0
	const WindowInfo *winInfo = (const WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	if (winInfo)
		stubSwapBuffers(winInfo, flags);
#endif
        return;
}

/**
 * Returns -1 on error
 */
GLint APIENTRY crWindowCreate( const char *dpyName, GLint visBits )
{
        CRASSERT(0);
#if 0
	stubInit();
	return stubNewWindow( dpyName, visBits );
#endif
        return 0;
}

void APIENTRY crWindowDestroy( GLint window )
{
        CRASSERT(0);
#if 0
	WindowInfo *winInfo = (WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	if (winInfo && winInfo->type == CHROMIUM) {
		stub.spu->dispatch_table.WindowDestroy( winInfo->spuWindow );
		crHashtableDelete(stub.windowTable, window, crFree);
	}
#endif
        return;
}

GLint APIENTRY crWindowReuse(  const char *dpyName, GLint visBits, GLint window )
{
    CRASSERT(0);
#if 0
    stubInit();
    return stubReuseWindow( dpyName, visBits, window );
#endif
    return 0;
}

GLint APIENTRY crWindowId( GLint crWindowId )
{
    CRASSERT(0);
#if 0
    return stub.spu->dispatch_table.WindowId( crWindowId );
#endif
    return 0;
}

void APIENTRY crWindowSize( GLint window, GLint w, GLint h )
{
        CRASSERT(0);
#if 0
	const WindowInfo *winInfo = (const WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	if (winInfo && winInfo->type == CHROMIUM)
		stub.spu->dispatch_table.WindowSize( window, w, h );
#endif
        return;
}

void APIENTRY crWindowPosition( GLint window, GLint x, GLint y )
{
        CRASSERT(0);
#if 0
	const WindowInfo *winInfo = (const WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	if (winInfo && winInfo->type == CHROMIUM)
		stub.spu->dispatch_table.WindowPosition( window, x, y );
#endif
        return;
}

void APIENTRY crWindowShow( GLint window, GLint flag )
{
        CRASSERT(0);
#if 0
	WindowInfo *winInfo = (WindowInfo *)
		crHashtableSearch(stub.windowTable, (unsigned int) window);
	if (winInfo && winInfo->type == CHROMIUM)
		stub.spu->dispatch_table.WindowShow( window, flag );
	winInfo->mapped = flag ? GL_TRUE : GL_FALSE;
#endif
        return;
}

void APIENTRY stub_GetChromiumParametervCR( GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values )
{
        CRASSERT(0);
#if 0
	char **ret;
	switch( target )
	{
		case GL_HEAD_SPU_NAME_CR:
			ret = (char **) values;
			*ret = stub.spu->name;
			return;
		default:
			stub.spu->dispatch_table.GetChromiumParametervCR( target, index, type, count, values );
			break;
	}
#endif
        return;
}
