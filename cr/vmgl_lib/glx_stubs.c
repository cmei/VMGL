/* GLX Wrappers that do more or less nothing.
 */
#if GLX_EXTRAS

GLXPbufferSGIX glXCreateGLXPbufferSGIX(Display *dpy, GLXFBConfigSGIX config,
																			 unsigned int width, unsigned int height,
																			 int *attrib_list)
{
	(void) dpy;
	(void) config;
	(void) width;
	(void) height;
	(void) attrib_list;
	crWarning("glXCreateGLXPbufferSGIX not implemented by VMGL/Chromium");
	return 0;
}

void glXDestroyGLXPbufferSGIX(Display *dpy, GLXPbuffer pbuf)
{
	(void) dpy;
	(void) pbuf;
	crWarning("glXDestroyGLXPbufferSGIX not implemented by VMGL/Chromium");
}

void glXSelectEventSGIX(Display *dpy, GLXDrawable drawable, unsigned long mask)
{
	(void) dpy;
	(void) drawable;
	(void) mask;
}

void glXGetSelectedEventSGIX(Display *dpy, GLXDrawable drawable, unsigned long *mask)
{
	(void) dpy;
	(void) drawable;
	(void) mask;
}

int glXQueryGLXPbufferSGIX(Display *dpy, GLXPbuffer pbuf,
													 int attribute, unsigned int *value)
{
	(void) dpy;
	(void) pbuf;
	(void) attribute;
	(void) value;
	crWarning("glXQueryGLXPbufferSGIX not implemented by VMGL/Chromium");
	return 0;
}

int glXGetFBConfigAttribSGIX(Display *dpy, GLXFBConfig config,
														 int attribute, int *value)
{
	(void) dpy;
	(void) config;
	(void) attribute;
	(void) value;
	crWarning("glXGetFBConfigAttribSGIX not implemented by VMGL/Chromium");
	return 0;
}

GLXFBConfigSGIX *glXChooseFBConfigSGIX(Display *dpy, int screen,
					 int *attrib_list, int *nelements)
{
	(void) dpy;
	(void) screen;
	(void) attrib_list;
	(void) nelements;
	crWarning("glXChooseFBConfigSGIX not implemented by VMGL/Chromium");
	return NULL;
}

GLXPixmap glXCreateGLXPixmapWithConfigSGIX(Display *dpy,
																					 GLXFBConfig config,
																					 Pixmap pixmap)
{
	(void) dpy;
	(void) config;
	(void) pixmap;
	crWarning("glXCreateGLXPixmapWithConfigSGIX not implemented by VMGL/Chromium");
	return 0;	}

GLXContext glXCreateContextWithConfigSGIX(Display *dpy, GLXFBConfig config,
																					int render_type,
																					GLXContext share_list,
																					Bool direct)
{
	(void) dpy;
	(void) config;
	(void) render_type;
	(void) share_list;
	(void) direct;
	crWarning("glXCreateContextWithConfigSGIX not implemented by VMGL/Chromium");
	return NULL;
}

XVisualInfo *glXGetVisualFromFBConfigSGIX(Display *dpy,
																					GLXFBConfig config)
{
	(void) dpy;
	(void) config;
	crWarning("glXGetVisualFromFBConfigSGIX not implemented by VMGL/Chromium");
	return NULL;
}

GLXFBConfigSGIX glXGetFBConfigFromVisualSGIX(Display *dpy, XVisualInfo *vis)
{
	(void) dpy;
	(void) vis;
	crWarning("glXGetFBConfigFromVisualSGIX not implemented by VMGL/Chromium");
	return NULL;
}

/*
 * GLX 1.3 functions
 */
GLXFBConfig *glXChooseFBConfig(Display *dpy, int screen, ATTRIB_TYPE *attrib_list, int *nelements)
{
	(void) dpy;
	(void) screen;
	(void) attrib_list;
	(void) nelements;
	crWarning("glXChooseFBConfig not implemented by VMGL/Chromium");
	return NULL;
}

GLXContext glXCreateNewContext(Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct)
{
	(void) dpy;
	(void) config;
	(void) render_type;
	(void) share_list;
	(void) direct;
	crWarning("glXCreateNewContext not implemented by VMGL/Chromium");
	return NULL;
}

GLXPbuffer glXCreatePbuffer(Display *dpy, GLXFBConfig config, ATTRIB_TYPE *attrib_list)
{
	(void) dpy;
	(void) config;
	(void) attrib_list;
	crWarning("glXCreatePbuffer not implemented by VMGL/Chromium");
	return 0;
}

GLXPixmap glXCreatePixmap(Display *dpy, GLXFBConfig config, Pixmap pixmap, ATTRIB_TYPE *attrib_list)
{
	(void) dpy;
	(void) config;
	(void) pixmap;
	(void) attrib_list;
	crWarning("glXCreatePixmap not implemented by VMGL/Chromium");
	return 0;
}

GLXWindow glXCreateWindow(Display *dpy, GLXFBConfig config, Window win, ATTRIB_TYPE *attrib_list)
{
	(void) dpy;
	(void) config;
	(void) win;
	(void) attrib_list;
	crWarning("glXCreateWindow not implemented by VMGL/Chromium");
	return 0;
}

void glXDestroyPbuffer(Display *dpy, GLXPbuffer pbuf)
{
	(void) dpy;
	(void) pbuf;
	crWarning("glXDestroyPbuffer not implemented by VMGL/Chromium");
}
void glXDestroyPixmap(Display *dpy, GLXPixmap pixmap)
{
	(void) dpy;
	(void) pixmap;
	crWarning("glXDestroyPixmap not implemented by VMGL/Chromium");
}

void glXDestroyWindow(Display *dpy, GLXWindow win)
{
	(void) dpy;
	(void) win;
	crWarning("glXDestroyWindow not implemented by VMGL/Chromium");
}

GLXDrawable glXGetCurrentReadDrawable(void)
{
	crWarning("glXGetCurrentReadDrawable not implemented by VMGL/Chromium");
	return 0;
}

int glXGetFBConfigAttrib(Display *dpy, GLXFBConfig config, int attribute, int *value)
{
	(void) dpy;
	(void) config;
	(void) attribute;
	(void) value;
	crWarning("glXGetFBConfigAttrib not implemented by VMGL/Chromium");
	return 0;
}

GLXFBConfig *glXGetFBConfigs(Display *dpy, int screen, int *nelements)
{
	(void) dpy;
	(void) screen;
	(void) nelements;
	crWarning("glXGetFBConfigs not implemented by VMGL/Chromium");
	return NULL;
}

void glXGetSelectedEvent(Display *dpy, GLXDrawable draw, unsigned long *event_mask)
{
	(void) dpy;
	(void) draw;
	(void) event_mask;
	crWarning("glXGetSelectedEvent not implemented by VMGL/Chromium");
}

XVisualInfo *glXGetVisualFromFBConfig(Display *dpy, GLXFBConfig config)
{
	(void) dpy;
	(void) config;
	crWarning("glXGetVisualFromFBConfig not implemented by VMGL/Chromium");
	return NULL;
}

Bool glXMakeContextCurrent(Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx)
{
	(void) display;
	(void) draw;
	(void) read;
	(void) ctx;
	crWarning("glXMakeContextCurrent not implemented by VMGL/Chromium");
	return 0;
}

int glXQueryContext(Display *dpy, GLXContext ctx, int attribute, int *value)
{
	(void) dpy;
	(void) ctx;
	(void) attribute;
	(void) value;
	crWarning("glXQueryContext not implemented by VMGL/Chromium");
	return 0;
}

void glXQueryDrawable(Display *dpy, GLXDrawable draw, int attribute, unsigned int *value)
{
	(void) dpy;
	(void) draw;
	(void) attribute;
	(void) value;
	crWarning("glXQueryDrawable not implemented by VMGL/Chromium");
}

void glXSelectEvent(Display *dpy, GLXDrawable draw, unsigned long event_mask)
{
	(void) dpy;
	(void) draw;
	(void) event_mask;
	crWarning("glXSelectEvent not implemented by VMGL/Chromium");
}


#endif /* GLX_EXTRAS */


#ifdef GLX_SGIX_video_resize
/* more dummy funcs.  These help when linking with older GLUTs */

int glXBindChannelToWindowSGIX(Display *dpy, int scrn, int chan, Window w)
{
	(void) dpy;
	(void) scrn;
	(void) chan;
	(void) w;
	return 0;
}

int glXChannelRectSGIX(Display *dpy, int scrn, int chan, int x , int y, int w, int h)
{
	(void) dpy;
	(void) scrn;
	(void) chan;
	(void) x;
	(void) y;
	(void) w;
	(void) h;
	return 0;
}

int glXQueryChannelRectSGIX(Display *dpy, int scrn, int chan, int *x, int *y, int *w, int *h)
{
	(void) dpy;
	(void) scrn;
	(void) chan;
	(void) x;
	(void) y;
	(void) w;
	(void) h;
	return 0;
}

int glXQueryChannelDeltasSGIX(Display *dpy, int scrn, int chan, int *dx, int *dy, int *dw, int *dh)
{
	(void) dpy;
	(void) scrn;
	(void) chan;
	(void) dx;
	(void) dy;
	(void) dw;
	(void) dh;
	return 0;
}

int glXChannelRectSyncSGIX(Display *dpy, int scrn, int chan, GLenum synctype)
{
	(void) dpy;
	(void) scrn;
	(void) chan;
	(void) synctype;
	return 0;
}

#endif /* GLX_SGIX_video_resize */

/**
 **  There is a problem with glXCopyContext.
 ** IRIX and Mesa both define glXCopyContext
 ** to have the mask argument being a 
 ** GLuint.  XFree 4 and oss.sgi.com
 ** define it to be an unsigned long.
 ** Solution: We don't support
 ** glXCopyContext anyway so we'll just
 ** #ifdef out the code.
 */
void
glXCopyContext( Display *dpy, GLXContext src, GLXContext dst, 
#if defined(PLAYSTATION2)
GLuint mask )
/*  #elif defined(SunOS)
  unsigned int mask ) */
#else
unsigned long mask )
#endif
{
	(void) dpy;
	(void) src;
	(void) dst;
	(void) mask;
	crWarning( "Unsupported GLX Call: glXCopyContext()" );
}

GLXPixmap glXCreateGLXPixmap( Display *dpy, XVisualInfo *vis, Pixmap pixmap )
{
	(void) dpy;
	(void) vis;
	(void) pixmap;

	stubInit();
	crWarning( "Unsupported GLX Call: glXCreateGLXPixmap()" );
	return (GLXPixmap) 0;
}

void glXDestroyGLXPixmap( Display *dpy, GLXPixmap pix )
{
	(void) dpy;
	(void) pix;
	crWarning( "Unsupported GLX Call: glXDestroyGLXPixmap()" );
}

