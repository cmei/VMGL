/* Copyright (c) 2001, Stanford University
 * All rights reserved
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#include "chromium.h"
#include "cr_error.h"
#include "cr_spu.h"
#include "cr_mem.h"
#include "cr_string.h"
#include "cr_list.h"
#include "cr_environment.h"
#include "stub.h"


static struct CRList* visualInfoList;

__attribute__((constructor))
static void init_glx(void)
{
    visualInfoList = crAllocList();
}

static int
Attrib( Display * dpy, XVisualInfo *visual, int attrib )
{
	int value = 0;
	stub.wsInterface.glXGetConfig( dpy, visual, attrib, &value );
	return value;
}

static VMMode getVMMode(Display *dpy)
{
    if (dpy == getNativeDisplay()) {
        return VM_NATIVE;
    }
    return VM_LIVE_MIGRATION;
}

static void
AddVisualInfo(Display *dpy, int screen, XVisualInfo* visual, VisualID visualid, int visBits, int * attribList)
{
        int i;
	VisualInfo *v = FindVisualInfo(visual);
        if (v) {
            v->visBits |= visBits;
            return;
        }
	v = (VisualInfo *) crAlloc(sizeof(VisualInfo));
        v->visual = visual;
	v->visBits = visBits;
        v->attribListSize = 0;
        v->attribList = NULL;
        v->vm_mode = getVMMode(dpy);
        if (attribList != NULL) {
            for (i = 0; attribList[i] != None; i++) {
                v->attribListSize += 1;
            }
            v->attribList = crAlloc(sizeof(v->attribList[0])*v->attribListSize);
            crMemcpy(v->attribList, attribList, sizeof(v->attribList[0])*v->attribListSize);
        } else if (visual != NULL) {
            int attrs[64];

            int n = 0;
            attrs[n++] = GLX_USE_GL;
            attrs[n++] = Attrib(dpy, visual, GLX_USE_GL);
            /* Ignored. Only visuals that can be rendered with GLX are considered. */

            attrs[n++] = GLX_BUFFER_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_BUFFER_SIZE);
            /* Must be followed by a nonnegative integer that indicates the desired color index buffer 
             * size. The smallest index buffer of at least the specified size is preferred. Ignored if 
             * GLX_RGBA is asserted. */

            attrs[n++] = GLX_LEVEL;
            attrs[n++] = Attrib(dpy, visual, GLX_LEVEL);
            /* Must be followed by an integer buffer-level specification. This specification is 
             * honored exactly. Buffer level zero corresponds to the main frame buffer of the display. 
             * Buffer level one is the first overlay frame buffer, level two the second overlay frame 
             * buffer, and so on. Negative buffer levels correspond to underlay frame buffers. */

            attrs[n++] = GLX_AUX_BUFFERS;
            attrs[n++] = Attrib(dpy, visual, GLX_AUX_BUFFERS);
            /* Must be followed by a nonnegative integer that indicates the desired number of 
             * auxiliary buffers. Visuals with the smallest number of auxiliary buffers that meets or 
             * exceeds the specified number are preferred. */

            attrs[n++] = GLX_RED_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_RED_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * the smallest available red buffer is preferred. Otherwise, the largest available red 
             * buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_GREEN_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_GREEN_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * the smallest available green buffer is preferred. Otherwise, the largest available 
             * green buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_BLUE_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_BLUE_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * the smallest available blue buffer is preferred. Otherwise, the largest available blue 
             * buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_ALPHA_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_ALPHA_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * the smallest available alpha buffer is preferred. Otherwise, the largest available 
             * alpha buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_DEPTH_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_DEPTH_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * visuals with no depth buffer are preferred. Otherwise, the largest available depth 
             * buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_STENCIL_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_STENCIL_SIZE);
            /* Must be followed by a nonnegative integer that indicates the desired number of stencil 
             * bitplanes. The smallest stencil buffer of at least the specified size is preferred. If 
             * the desired value is zero, visuals with no stencil buffer are preferred. */

            attrs[n++] = GLX_ACCUM_RED_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_ACCUM_RED_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * visuals with no red accumulation buffer are preferred. Otherwise, the largest possible 
             * red accumulation buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_ACCUM_GREEN_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_ACCUM_GREEN_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * visuals with no green accumulation buffer are preferred. Otherwise, the largest 
             * possible green accumulation buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_ACCUM_BLUE_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_ACCUM_BLUE_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * visuals with no blue accumulation buffer are preferred. Otherwise, the largest possible 
             * blue accumulation buffer of at least the minimum size is preferred. */

            attrs[n++] = GLX_ACCUM_ALPHA_SIZE;
            attrs[n++] = Attrib(dpy, visual, GLX_ACCUM_ALPHA_SIZE);
            /* Must be followed by a nonnegative minimum size specification. If this value is zero, 
             * visuals with no alpha accumulation buffer are preferred. Otherwise, the largest 
             * possible alpha accumulation buffer of at least the minimum size is preferred. */

            if (Attrib(dpy, visual, GLX_RGBA))
                attrs[n++] = GLX_RGBA;
            /* If present, only TrueColor and DirectColor visuals are considered. Otherwise, only 
             * PseudoColor and StaticColor visuals are considered. */

            if (Attrib(dpy, visual, GLX_DOUBLEBUFFER))
                attrs[n++] = GLX_DOUBLEBUFFER;
            /* If present, only double-buffered visuals are considered. Otherwise, only 
             * single-buffered visuals are considered. */

            if (Attrib(dpy, visual, GLX_STEREO))
                attrs[n++] = GLX_STEREO;
            /* If present, only stereo visuals are considered. Otherwise, only monoscopic visuals are 
             * considered. */

            attrs[n++] = None;

            v->attribList = crAlloc(sizeof(attrs[0])*n);
            crMemcpy(v->attribList, attrs, sizeof(attrs[0])*n);
            v->attribListSize = n;

        } else {
            crError("Cannot track state of XVisualInfo; cannot determine attribList used to construct it");
        }

        crListPushBack(visualInfoList, v);
}

/* Return true iff A is a subset of the bits set in B: 
 * e.g.
 * A:      101010
 * B:      101000
 * ~B:     010111
 * A & ~B: 000010
 *
 * A:      101000
 * B:      101010
 * ~B:     010101
 * A & ~B: 000000
*/
#define IS_SUBSET_OF(A, B) (((A) & (~(B))) == 0)

VisualInfo *
FindVisualInfo(XVisualInfo* vis)
{
    CRListIterator *iter;
    VisualInfo *v;
    for ( iter = crListBegin( visualInfoList ); iter != crListEnd( visualInfoList ); iter = crListNext( iter ) ) {
        v = crListElement(iter);
        if (v->visual == vis)
            return v;
    }
    return NULL;
}

static const char * vm_mode_str(VMMode vm_mode)
{
    switch (vm_mode)
    {
        case VM_LIVE_MIGRATION:
            return "VM_LIVE_MIGRATION";
        case VM_NATIVE:
            return "VM_NATIVE";
        default:
            CRASSERT(0);
            return NULL;
    }
}

/**
 * Set this to 1 if you want to build stub functions for the
 * GL_SGIX_pbuffer and GLX_SGIX_fbconfig extensions.
 * This used to be disabled, due to "messy compilation issues",
 * according to the earlier comment; but they're needed just
 * to resolve symbols for OpenInventor applications, and I
 * haven't found any reference to exactly what the "messy compilation
 * issues" are, so I'm re-enabling the code by default.
 */
#define GLX_EXTRAS 1

/**
 * Prototypes, in case they're not in glx.h or glxext.h
 * Unfortunately, there's some inconsistency between the extension
 * specs, and the SGI, NVIDIA, XFree86 and common glxext.h header
 * files.
 */
#if defined(GLX_GLXEXT_VERSION)
/* match glxext.h, XFree86, Mesa */
#define ATTRIB_TYPE const int
#else
#define ATTRIB_TYPE int
#endif

#if GLX_EXTRAS
GLXPbufferSGIX glXCreateGLXPbufferSGIX(Display *dpy, GLXFBConfigSGIX config, unsigned int width, unsigned int height, int *attrib_list);
int glXQueryGLXPbufferSGIX(Display *dpy, GLXPbuffer pbuf, int attribute, unsigned int *value);
GLXFBConfigSGIX *glXChooseFBConfigSGIX(Display *dpy, int screen, int *attrib_list, int *nelements);

void glXDestroyGLXPbufferSGIX(Display *dpy, GLXPbuffer pbuf);
void glXSelectEventSGIX (Display *dpy, GLXDrawable drawable, unsigned long mask);
void glXGetSelectedEventSGIX (Display *dpy, GLXDrawable drawable, unsigned long *mask);

GLXFBConfigSGIX glXGetFBConfigFromVisualSGIX(Display *dpy, XVisualInfo *vis);
XVisualInfo *glXGetVisualFromFBConfigSGIX(Display *dpy, GLXFBConfig config);
GLXContext glXCreateContextWithConfigSGIX(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
GLXPixmap glXCreateGLXPixmapWithConfigSGIX(Display *dpy, GLXFBConfig config, Pixmap pixmap);
int glXGetFBConfigAttribSGIX(Display *dpy, GLXFBConfig config, int attribute, int *value);

/*
 * GLX 1.3 functions
 */
GLXFBConfig *glXChooseFBConfig(Display *dpy, int screen, ATTRIB_TYPE *attrib_list, int *nelements);
GLXPbuffer glXCreatePbuffer(Display *dpy, GLXFBConfig config, ATTRIB_TYPE *attrib_list);
GLXPixmap glXCreatePixmap(Display *dpy, GLXFBConfig config, Pixmap pixmap, ATTRIB_TYPE *attrib_list);
GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, ATTRIB_TYPE *attrib_list);
GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
void glXDestroyPbuffer(Display *dpy, GLXPbuffer pbuf);
void glXDestroyPixmap(Display *dpy, GLXPixmap pixmap);
void glXDestroyWindow(Display *dpy, GLXWindow win);
GLXDrawable glXGetCurrentReadDrawable(void);
int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value);
GLXFBConfig *glXGetFBConfigs(Display *dpy, int screen, int *nelements);
void glXGetSelectedEvent(Display *dpy, GLXDrawable draw, unsigned long *event_mask);
XVisualInfo *glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config);
Bool glXMakeContextCurrent(Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
int glXQueryContext(Display *dpy, GLXContext ctx, int attribute, int *value);
void glXQueryDrawable(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
void glXSelectEvent(Display *dpy, GLXDrawable draw, unsigned long event_mask);

 

#endif /* GLX_EXTRAS */



/**
 * Return string for a GLX error code
 */
static const char *glx_error_string(int err)
{
	static const char *glxErrors[] = {
		"none",
		"GLX_BAD_SCREEN",
		"GLX_BAD_ATTRIBUTE",
		"GLX_NO_EXTENSION",
		"GLX_BAD_VISUAL",
		"GLX_BAD_CONTEXT",
		"GLX_BAD_VALUE",
		"GLX_BAD_ENUM"
	};
	if (err > 0 && err < 8) {
		return glxErrors[err];
	}
	else {
		static char tmp[100];
		sprintf(tmp, "0x%x", err);
		return tmp;
	}
}

/* Given an XVisualInfo structure, try to figure out what its 
 * OpenGL capabilities are, if we have a native OpenGL.
 * Returns 0 if no information is available.
 */
static struct {
	int gl_attrib;
	char *attrib_name;
	enum {TEST_TRUE, TEST_GREATER_0} test;
	int match_vis_bits;
} attrib_map[] = {
	{GLX_RGBA, "GLX_RGBA", TEST_TRUE, CR_RGB_BIT},
	{GLX_DOUBLEBUFFER, "GLX_DOUBLEBUFFER", TEST_TRUE, CR_DOUBLE_BIT},
	{GLX_STEREO, "GLX_STEREO", TEST_TRUE, CR_STEREO_BIT},
	{GLX_LEVEL, "GLX_LEVEL", TEST_GREATER_0, CR_OVERLAY_BIT},
	{GLX_ALPHA_SIZE, "GLX_ALPHA_SIZE", TEST_GREATER_0, CR_ALPHA_BIT},
	{GLX_DEPTH_SIZE, "GLX_DEPTH_SIZE", TEST_GREATER_0, CR_DEPTH_BIT},
	{GLX_STENCIL_SIZE, "GLX_STENCIL_SIZE", TEST_GREATER_0, CR_STENCIL_BIT},
	{GLX_ACCUM_RED_SIZE, "GLX_ACCUM_RED_SIZE", TEST_GREATER_0, CR_ACCUM_BIT},
	{GLX_SAMPLE_BUFFERS_SGIS, "GLX_SAMPLE_BUFFERS_SGIS", TEST_GREATER_0, CR_MULTISAMPLE_BIT},
};
static int QueryVisBits(Display *dpy, XVisualInfo *vis)
{
	int visBits = 0;
	int foo, bar, return_val, value;
	unsigned int i;

	/* We can only query the OpenGL capabilities if we actually
	 * have a native OpenGL underneath us.  Without it, we can't
	 * get at all the actual OpenGL characteristics.
	 */
	if (!stub.haveNativeOpenGL) return 0;

	if (!stub.wsInterface.glXQueryExtension(dpy, &foo, &bar)) return 0;

	/* If we don't have the GLX_USE_GL attribute, we've failed. */
	return_val = stub.wsInterface.glXGetConfig(dpy, vis, GLX_USE_GL, &value);
	if (return_val) {
		crDebug("native glXGetConfig returned %d (%s) at %s line %d",
			return_val, glx_error_string(return_val), __FILE__, __LINE__);
		return 0;
	}
	if (value == 0) {
		crDebug("visual ID 0x%x doesn't support OpenGL at %s line %d",
			(int) vis->visual->visualid, __FILE__, __LINE__);
		return 0;
	}

	for (i = 0; i < sizeof(attrib_map)/sizeof(attrib_map[0]); i++) {
		return_val = stub.wsInterface.glXGetConfig(dpy, vis, attrib_map[i].gl_attrib, &value);
		if (return_val) {
			crDebug("native glXGetConfig(%s) returned %d (%s) at %s line %d",
				attrib_map[i].attrib_name, return_val, glx_error_string(return_val), __FILE__, __LINE__);
			return 0;
		}

		switch(attrib_map[i].test) {
			case TEST_TRUE:
				if (value)
					visBits |= attrib_map[i].match_vis_bits;
				break;

			case TEST_GREATER_0:
				if (value > 0)
					visBits |= attrib_map[i].match_vis_bits;
				break;

			default:
				crWarning("illegal attribute map test for %s at %s line %d", 
					attrib_map[i].attrib_name, __FILE__, __LINE__);
				return 0;
		}
	}

	return visBits;
}



XVisualInfo *
glXChooseVisual( Display *dpy, int screen, int *attribList )
{
	XVisualInfo *vis;
	int *attrib;
	int visBits = 0;

	stubInit();

	for (attrib = attribList; *attrib != None; attrib++)
	{
		switch (*attrib)
		{
			case GLX_USE_GL:
				/* ignored, this is mandatory */
				break;

			case GLX_BUFFER_SIZE:
				/* this is for color-index visuals, which we don't support */
				attrib++;
				break;

			case GLX_LEVEL:
				if (attrib[1] > 0)
					visBits |= CR_OVERLAY_BIT;
				attrib++;
				break;

			case GLX_RGBA:
				visBits |= CR_RGB_BIT;
				break;

			case GLX_DOUBLEBUFFER:
				visBits |= CR_DOUBLE_BIT;
				break;

			case GLX_STEREO:
				visBits |= CR_STEREO_BIT;
				/*
				crWarning( "glXChooseVisual: stereo unsupported" );
				return NULL;
				*/
				break;

			case GLX_AUX_BUFFERS:
				{
					int aux_buffers = attrib[1];
					if (aux_buffers != 0)
					{
						crWarning("glXChooseVisual: aux_buffers=%d unsupported",
											aux_buffers);
						return NULL;
					}
				}
				attrib++;
				break;

			case GLX_RED_SIZE:
			case GLX_GREEN_SIZE:
			case GLX_BLUE_SIZE:
				if (attrib[1] > 0)
					visBits |= CR_RGB_BIT;
				attrib++;
				break;

			case GLX_ALPHA_SIZE:
				if (attrib[1] > 0)
					visBits |= CR_ALPHA_BIT;
				attrib++;
				break;

			case GLX_DEPTH_SIZE:
				if (attrib[1] > 0)
					visBits |= CR_DEPTH_BIT;
				attrib++;
				break;

			case GLX_STENCIL_SIZE:
				if (attrib[1] > 0)
					visBits |= CR_STENCIL_BIT;
				attrib++;
				break;

			case GLX_ACCUM_RED_SIZE:
			case GLX_ACCUM_GREEN_SIZE:
			case GLX_ACCUM_BLUE_SIZE:
			case GLX_ACCUM_ALPHA_SIZE:
				if (attrib[1] > 0)
					visBits |= CR_ACCUM_BIT;
				attrib++;
				break;

			case GLX_SAMPLE_BUFFERS_SGIS: /* aka GLX_SAMPLES_ARB */
				if (attrib[1] > 0)
					visBits |= CR_MULTISAMPLE_BIT;
				attrib++;
				break;
			case GLX_SAMPLES_SGIS: /* aka GLX_SAMPLES_ARB */
				/* just ignore value for now, we'll try to get 4 samples/pixel */
				if (attrib[1] > 4)
					return NULL;
				visBits |= CR_MULTISAMPLE_BIT;
				attrib++;
				break;

#ifdef GLX_VERSION_1_3
			case GLX_X_VISUAL_TYPE:
			case GLX_TRANSPARENT_TYPE_EXT:
			case GLX_TRANSPARENT_INDEX_VALUE_EXT:
			case GLX_TRANSPARENT_RED_VALUE_EXT:
			case GLX_TRANSPARENT_GREEN_VALUE_EXT:
			case GLX_TRANSPARENT_BLUE_VALUE_EXT:
			case GLX_TRANSPARENT_ALPHA_VALUE_EXT:
				/* ignore */
				crWarning("glXChooseVisual: ignoring attribute 0x%x", *attrib);
				attrib++;
				break;
#endif

			default:
				crWarning( "glXChooseVisual: bad attrib=0x%x", *attrib );
				return NULL;
		}
	}

	if ((visBits & CR_RGB_BIT) == 0 && (visBits & CR_OVERLAY_BIT) == 0)
	{
		/* normal layer, color index mode not supported */
		crWarning( "glXChooseVisual: didn't request RGB visual?" );
		return NULL;
	}

	vis = crChooseVisual(&stub.wsInterface, dpy, screen, GL_FALSE, visBits);
	if (!vis && (visBits & CR_STEREO_BIT)) {
		/* try non-stereo */
		visBits &= ~CR_STEREO_BIT;
		vis = crChooseVisual(&stub.wsInterface, dpy, screen, GL_FALSE, visBits);
	}

	if (vis) {
		AddVisualInfo(dpy, screen, vis, vis->visual->visualid, visBits, attribList);
	}
	return vis;
}

/**
 * Get the display string for the given display pointer.
 * Never return just ":0.0".  In that case, prefix with our host name.
 */
static void
stubGetDisplayString( Display *dpy, char *nameResult, int maxResult )
{
	const char *dpyName = DisplayString(dpy);
	char host[1000];
#if 0
	if (dpyName[0] == ':')
	{
		crGetHostname(host, 1000);
	}
	else
#endif
	{
	  host[0] = 0;
	}
	if (crStrlen(host) + crStrlen(dpyName) >= maxResult - 1)
	{
		/* return null string */
		crWarning("Very long host / display name string in stubDisplayString!");
		nameResult[0] = 0;
	}
	else
	{
		/* return host concatenated with dpyName */
		crStrcpy(nameResult, host);
		crStrcat(nameResult, dpyName);
	}
}

GLint GetVisBits(VisualInfo* visualInfo)
{
    /* if (visualInfo) */
    return visualInfo->visBits;

/* Currently, we're assuming we always have a visualInfo */
#if 0
    GLint visBits = CR_RGB_BIT | CR_DOUBLE_BIT | CR_DEPTH_BIT; /* default vis */
    if (stub.force_pbuffers) {
        crDebug("Forcing use of Pbuffers");
        visBits |= CR_PBUFFER_BIT;
    }

    return visBits;
#endif
}

DisplayInfo* trans_DisplayInfo(Display *dpy)
{
    DisplayInfo* displayInfo = crHashtableSearch(stub.displayTable, (unsigned long) dpy);
    CRASSERT(displayInfo);
    CRASSERT(displayInfo->currentDpy);
    return displayInfo;
}
VisualInfo* trans_VisualInfo(XVisualInfo *vis)
{
    VisualInfo* visualInfo = crHashtableSearch(stub.visualTable, (unsigned long) vis);
    CRASSERT(visualInfo);
    CRASSERT(visualInfo->currentVisual);
    return visualInfo;
}

GLXContext
glXCreateContext(Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct)
{
	ContextInfo *contextInfo;
        GLint visBits;

	stubInit();

	CRASSERT(stub.contextTable);

        DisplayInfo* displayInfo = trans_DisplayInfo(dpy);
        VisualInfo* visualInfo = trans_VisualInfo(vis);

        visBits = GetVisBits(visualInfo);

	contextInfo = add_ContextInfo(displayInfo, visualInfo, visBits, UNDECIDED, direct, shareList);
        CRASSERT(contextInfo);

	return (GLXContext) contextInfo->id;
}


void glXDestroyContext( Display *dpy, GLXContext ctx )
{
	(void) dpy;
	stubDestroyContext( (unsigned long) ctx );
}


Bool glXMakeCurrent( Display *dpy, GLXDrawable drawable, GLXContext ctx )
{
    DisplayInfo* displayInfo;
    ContextInfo *context = NULL;
    WindowInfo *window = NULL;
    Bool retVal;

    crDebug("glXMakeCurrent(%p, 0x%x, 0x%lx)", (void *) dpy, (int) drawable, (unsigned long) ctx);

    if (ctx && drawable) {
        context = (ContextInfo *) crHashtableSearch(stub.contextTable, (unsigned long) ctx);
        window = stubGetWindowInfo(drawable);

        if (context && context->type == UNDECIDED) {
            XSync(dpy, 0); /* sync to force window creation on the server */
        }
    }

    displayInfo = crHashtableSearch(stub.displayTable, (unsigned long) dpy);
    CRASSERT(displayInfo);

    stub.currentDisplay = displayInfo;
    stub.currentDrawable = drawable;

    retVal = stubMakeCurrent(window, context);
    return retVal;
}


int glXGetConfig( Display *dpy, XVisualInfo *vis, int attrib, int *value )
{
	VisualInfo *v;
	int visBits;

	if (!vis) {
		/* SGI OpenGL Performer hits this */
		crWarning("glXGetConfig called with NULL XVisualInfo");
		return GLX_BAD_VISUAL;
	}

	v = FindVisualInfo(vis);
	if (v) {
		visBits = v->visBits;
	}
	else {
		visBits = 0;
	}

	stubInit();

	/* try to satisfy this request with the native glXGetConfig() */
	if (stub.haveNativeOpenGL)
	{
		int foo, bar;
		int return_val;

		if (stub.wsInterface.glXQueryExtension(dpy, &foo, &bar))
		{
			return_val = stub.wsInterface.glXGetConfig( dpy, vis, attrib, value );
			if (return_val)
			{
				crDebug("Native glXGetConfig returned %s",
								glx_error_string(return_val));
			}
			return return_val;
		}
	}

	/*
	 * If the GLX application chooses its visual via a bunch of calls to
	 * glXGetConfig, instead of by calling glXChooseVisual, we need to keep
	 * track of which attributes are queried to help satisfy context creation
	 * later.
	 */
	switch ( attrib ) {

		case GLX_USE_GL:
			*value = 1;
			break;

		case GLX_BUFFER_SIZE:
			*value = 32;
			break;

		case GLX_LEVEL:
			visBits |= CR_OVERLAY_BIT;
			*value = (visBits & CR_OVERLAY_BIT) ? 1 : 0;
			break;

		case GLX_RGBA:
			visBits |= CR_RGB_BIT;
			*value = 1;
			break;

		case GLX_DOUBLEBUFFER:
			*value = 1;
			break;

		case GLX_STEREO:
			*value = 1;
			break;

		case GLX_AUX_BUFFERS:
			*value = 0;
			break;

		case GLX_RED_SIZE:
			*value = 8;
			break;

		case GLX_GREEN_SIZE:
			*value = 8;
			break;

		case GLX_BLUE_SIZE:
			*value = 8;
			break;

		case GLX_ALPHA_SIZE:
			visBits |= CR_ALPHA_BIT;
			*value = (visBits & CR_ALPHA_BIT) ? 8 : 0;
			break;

		case GLX_DEPTH_SIZE:
			visBits |= CR_DEPTH_BIT;
			*value = 16;
			break;

		case GLX_STENCIL_SIZE:
			visBits |= CR_STENCIL_BIT;
			*value = 8;
			break;

		case GLX_ACCUM_RED_SIZE:
			visBits |= CR_ACCUM_BIT;
			*value = 16;
			break;

		case GLX_ACCUM_GREEN_SIZE:
			visBits |= CR_ACCUM_BIT;
			*value = 16;
			break;

		case GLX_ACCUM_BLUE_SIZE:
			visBits |= CR_ACCUM_BIT;
			*value = 16;
			break;

		case GLX_ACCUM_ALPHA_SIZE:
			visBits |= CR_ACCUM_BIT;
			*value = 16;
			break;

		case GLX_SAMPLE_BUFFERS_SGIS:
			visBits |= CR_MULTISAMPLE_BIT;
			*value = 0;  /* fix someday */
			break;

		case GLX_SAMPLES_SGIS:
			visBits |= CR_MULTISAMPLE_BIT;
			*value = 0;  /* fix someday */
			break;

		case GLX_VISUAL_CAVEAT_EXT:
			*value = GLX_NONE_EXT;
			break;
#ifdef SunOS 
                /* 
                  I don't think this is even a valid attribute for glxGetConfig.  
                  No idea why this gets called under SunOS but we simply ignore it 
                */
                case GLX_X_VISUAL_TYPE:
                  crWarning ("Ignoring Unsuported GLX Call: glxGetConfig with attrib 0x%x", attrib);
                  break;
#endif  
		case GLX_TRANSPARENT_TYPE:
			*value = GLX_NONE_EXT;
			break;
		case GLX_TRANSPARENT_INDEX_VALUE:
			*value = 0;
			break;
		case GLX_TRANSPARENT_RED_VALUE:
			*value = 0;
			break;
		case GLX_TRANSPARENT_GREEN_VALUE:
			*value = 0;
			break;
		case GLX_TRANSPARENT_BLUE_VALUE:
			*value = 0;
			break;
		case GLX_TRANSPARENT_ALPHA_VALUE:
			*value = 0;
			break;
		default:
			crWarning( "Unsupported GLX Call: glXGetConfig with attrib 0x%x", attrib );
			return GLX_BAD_ATTRIBUTE;
		
	}

        if (!v) {
            AddVisualInfo(dpy, DefaultScreen(dpy), vis, vis->visual->visualid, visBits, NULL);
        }

	return 0;
}

GLXContext glXGetCurrentContext( void )
{
	if (stub.currentContext)
		return (GLXContext) stub.currentContext->id;
	else
		return (GLXContext) NULL;
}

GLXDrawable glXGetCurrentDrawable( void )
{
	return stub.currentDrawable;
}

Display * getNativeDisplay(void)
{
    return stub.currentDisplay ? stub.currentDisplay->currentDpy : NULL;
}
Display *glXGetCurrentDisplay( void )
{
    return stub.currentDisplay ? stub.currentDisplay->dpy : NULL;
}

Bool glXIsDirect( Display *dpy, GLXContext ctx )
{
	(void) dpy;
	(void) ctx;
	return True;
}

Bool glXQueryExtension( Display *dpy, int *errorBase, int *eventBase )
{
	(void) dpy;
	(void) errorBase;
	(void) eventBase;
	return 1; /* You BET we do... */
}

Bool glXQueryVersion( Display *dpy, int *major, int *minor )
{
	(void) dpy;
	*major = 1;
	*minor = 3;
	return 1;
}

void glXSwapBuffers( Display *dpy, GLXDrawable drawable )
{
	const WindowInfo *window = stubGetWindowInfo(drawable);
	stubSwapBuffers( window, 0 );
}

void glXUseXFont( Font font, int first, int count, int listBase )
{
	if (stub.currentContext->type == CHROMIUM)
	{
		Display *dpy = stub.wsInterface.glXGetCurrentDisplay();
		if (dpy) {
			stubUseXFont( dpy, font, first, count, listBase );
		}
		else {
			dpy = XOpenDisplay(NULL);
			if (!dpy)
				return;
			stubUseXFont( dpy, font, first, count, listBase );
			XCloseDisplay(dpy);
		}
	} else
		stub.wsInterface.glXUseXFont( font, first, count, listBase );
}

void glXWaitGL( void )
{
	static int first_call = 1;

	if ( first_call )
	{
		crDebug( "Ignoring unsupported GLX call: glXWaitGL()" );
		first_call = 0;
	}
}

void glXWaitX( void )
{
	static int first_call = 1;

	if ( first_call )
	{
		crDebug( "Ignoring unsupported GLX call: glXWaitX()" );
		first_call = 0;
	}
}

const char *glXQueryExtensionsString( Display *dpy, int screen )
{
	/* XXX maybe also advertise GLX_SGIS_multisample? */
	static const char *retval = "GLX_ARB_multisample";
	(void) dpy;
	(void) screen;

	return retval;
}

const char *glXGetClientString( Display *dpy, int name )
{
	const char *retval;
	(void) dpy;
	(void) name;

	switch ( name ) {

		case GLX_VENDOR:
			retval  = "VMGL";
			break;

		case GLX_VERSION:
			retval  = "1.2 VMGL";
			break;

		case GLX_EXTENSIONS:
			retval  = "";
			break;

		default:
			retval  = NULL;
	}

	return retval;
}

const char *glXQueryServerString( Display *dpy, int screen, int name )
{
	const char *retval;
	(void) dpy;
	(void) screen;

	switch ( name ) {

		case GLX_VENDOR:
			retval  = "VMGL";
			break;

		case GLX_VERSION:
			retval  = "1.2 VMGL";
			break;

		case GLX_EXTENSIONS:
			retval  = "";
			break;

		default:
			retval  = NULL;
	}

	return retval;
}

CR_GLXFuncPtr glXGetProcAddressARB( const GLubyte *name )
{
	return (CR_GLXFuncPtr) crGetProcAddress( (const char *) name );
}

CR_GLXFuncPtr glXGetProcAddress( const GLubyte *name )
{
	return (CR_GLXFuncPtr) crGetProcAddress( (const char *) name );
}

static WindowInfo* 
stubCreateWindowInfo(Window ret,
    Display*		display,
    Window		parent,
    int			x,
    int			y,
    unsigned int	width,
    unsigned int	height,
    unsigned int	border_width,
    int			depth,
    unsigned int	class,
    Visual*		visual,
    unsigned long	valuemask,
    XSetWindowAttributes*	attributes)
{
    WindowInfo *winInfo = (WindowInfo *) crCalloc(sizeof(WindowInfo));
    if (!winInfo) {
        crDebug("Failed to allocate WindowInfo");
        return NULL;
    }
#ifdef GLX
    crStrncpy(winInfo->dpyName, DisplayString(display), MAX_DPY_NAME);
    winInfo->dpyName[MAX_DPY_NAME-1] = 0;
    winInfo->dpy = display;
    if (hasNativeDisplay()) {
        winInfo->nativeDrawable = stub.wsInterface.XCreateWindow(getNativeDisplay(), 
                parent,
                x, y,
                width, height,
                border_width,
                depth,
                class,
                visual,
                valuemask,
                attributes);
        CRASSERT(winInfo->nativeDrawable);
    } else {
        winInfo->nativeDrawable = 0;
    }
#endif
    winInfo->drawable = ret;
    winInfo->type = UNDECIDED;
    winInfo->spuWindow = -1;
    winInfo->mapped = -1; /* don't know */
    CRASSERT(crHashtableSearch(stub.windowTable, (unsigned int) ret) == NULL);
    crHashtableAdd(stub.windowTable, (unsigned int) ret, winInfo);
    return winInfo;
}


Window XCreateWindow(
    Display*		display,
    Window		parent,
    int			x,
    int			y,
    unsigned int	width,
    unsigned int	height,
    unsigned int	border_width,
    int			depth,
    unsigned int	class,
    Visual*		visual,
    unsigned long	valuemask,
    XSetWindowAttributes*	attributes
)
{
    CRASSERT(stub.wsInterface.XCreateWindow);
    stubInit();

    Window ret = stub.wsInterface.XCreateWindow(display,
            parent,
            x,
            y,
            width,
            height,
            border_width,
            depth,
            class,
            visual,
            valuemask,
            attributes);
    if (!ret)
        return 0;
    WindowInfo * winInfo = stubCreateWindowInfo(ret, display,
            parent,
            x,
            y,
            width,
            height,
            border_width,
            depth,
            class,
            visual,
            valuemask,
            attributes);
    if (!winInfo)
        return 0;
    return ret;
}

extern int XDestroyWindow(
    Display*		display,
    Window		w
)
{
    int ret;

    CRASSERT(stub.wsInterface.XDestroyWindow);
    stubInit();

    WindowInfo *winInfo = stubGetWindowInfo(w);
    if (winInfo->nativeDrawable && stub.vm_mode == VM_NATIVE) {
        CR_DEBUG(DB_X11, "XDestroy native window: dpy = 0x%lx, nativeDrawable = 0x%lx", (unsigned long) getNativeDisplay(), winInfo->nativeDrawable);
        ret = stub.wsInterface.XDestroyWindow(getNativeDisplay(), winInfo->nativeDrawable);
        CRASSERT(ret == 0);
    } else {
        CR_DEBUG(DB_X11, "XDestroy migration window: dpy = 0x%lx, drawable = 0x%lx", (unsigned long) winInfo->dpy, winInfo->drawable);
        ret = stub.wsInterface.XDestroyWindow(winInfo->dpy, winInfo->drawable);
        CRASSERT(ret == 0);
    }
    crHashtableDelete(stub.windowTable, (unsigned int) winInfo->drawable, NULL);
    crFree(winInfo);
    return ret;
}

Display *XOpenDisplay(
    _Xconst char*	 display_name
)
{
    Display * ret;

    CRASSERT(stub.wsInterface.XDestroyWindow);
    stubInit();

    ret = stub.wsInterface.XOpenDisplay(display_name);
    if (!ret)
        return ret;

    DisplayInfo* displayInfo = add_DisplayInfo(ret, display_name);
    CRASSERT(displayInfo);

    CRASSERT(crHashtableSearch(stub.displayTable, (unsigned long) ret) == NULL);
    crHashtableAdd(stub.displayTable, (unsigned long) ret, displayInfo);

    if (stub.currentDisplay == NULL)
        stub.currentDisplay = displayInfo;

    return ret;
}

DisplayInfo* add_DisplayInfo(Display * ret, _Xconst char* display_name)
{
    DisplayInfo* displayInfo = crAlloc(sizeof(DisplayInfo));
    if (!displayInfo)
        return NULL;

    const char * name = display_name ? display_name : crGetenv("DISPLAY");
    CRASSERT(name);

    displayInfo->dpy = ret;
    displayInfo->currentDpy = ret;
    crStrncpy(displayInfo->currentDisplay, name, sizeof(displayInfo->currentDisplay));
    crStrncpy(displayInfo->display, name, sizeof(displayInfo->display));

    return displayInfo;
}
void remove_DisplayInfo(DisplayInfo * displayInfo)
{
    crFree(displayInfo);
}

int XCloseDisplay(
    Display*		display
)
{
    int ret;

    DisplayInfo* displayInfo = trans_DisplayInfo(display);

    ret = stub.wsInterface.XCloseDisplay(displayInfo->currentDpy);
    CRASSERT(ret == 0);

    crHashtableDelete(stub.displayTable, (unsigned long) display, NULL);

    if (stub.currentDisplay == displayInfo)
        stub.currentDisplay = NULL;

    remove_DisplayInfo(displayInfo);

    return ret;
}
