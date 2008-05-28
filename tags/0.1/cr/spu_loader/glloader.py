# Copyright (c) 2001, Stanford University
# All rights reserved.
#
# See the file LICENSE.txt for information on redistributing this software.


import sys
sys.path.append( "../glapi_parser" )
import apiutil


keys = apiutil.GetDispatchedFunctions()

apiutil.CopyrightC()

print """
/* DO NOT EDIT - THIS FILE GENERATED BY THE glloader.py SCRIPT */
#include "cr_error.h"
#include "cr_dll.h"
#include "cr_spu.h"
#include "cr_string.h"
#include "cr_error.h"
#include "cr_environment.h"

#include <stdio.h>
#if defined(Linux) || defined (SunOS) || defined(FreeBSD)
#define SYSTEM_GL "libGL.so.1"
typedef void (*glxfuncptr)();
extern glxfuncptr glxGetProcAddressARB( const GLubyte *name );
#else
#error I don't know where your system's GL lives.  Too bad.
#endif

static CRDLL *glDll = NULL;

#define GLLOADER_APIENTRY


/*
 * Add an entry to the SPUNamedFunctionTable
 */
static int
fillin( SPUNamedFunctionTable *entry, const char *funcName, SPUGenericFunction funcPtr )
{
	if (funcPtr) {
		entry->name = crStrdup( funcName );
		entry->fn = funcPtr;
		return 1;
	}
	return 0;
}

static int FileExists(const char *directory, const char *filename)
{
	FILE *f;
	char fullFilename[8096];

	crStrcpy(fullFilename, directory);
	crStrcat(fullFilename, "/");
	crStrcat(fullFilename, filename);

	f = fopen(fullFilename, "r");
	if (f) {
		fclose(f);
		return 1;
	}
	else {
	    return 0;
	}
}


/*
 * Locate the native OpenGL library, open it and return shared library
 * handle.
 */
static CRDLL *
__findSystemLib( const char *provided_system_path, const char *lib )
{
	CRDLL *dll;
	char system_path[8096];
	if (provided_system_path && (crStrlen(provided_system_path) > 0) )
	{
		crStrcpy( system_path, provided_system_path );
	}
	else
	{
#if defined(PLAYSTATION2)
		crStrcpy( system_path, "/usr/X11R6/lib" );
#else
		/* On RedHat 9, the correct default system directory
		 * is /usr/lib/tls/ (and if /usr/lib/ is used,
		 * the dynamic loader will generate a floating point
		 * exception SIGFPE).  On other systems, including
		 * earlier versions of RedHat, the OpenGL library
		 * lives in /usr/lib.   We'll use the /usr/lib/tls/
		 * version if it exists; otherwise, we'll use /usr/lib.
		 */
		crStrcpy(system_path, "/usr/lib");
#if defined(__linux__) && (defined(__amd64__) || defined(__x86_64__))
                if (sizeof(void *) == 8 && FileExists("/usr/lib64", lib)) {
			crStrcat(system_path, "64");
		}
#endif
		if (FileExists("/usr/lib/tls", lib) ||
		    FileExists("/usr/lib64/tls", lib)) {
			crStrcat(system_path, "/tls");
		}
#endif
	}
	crStrcat( system_path, "/" );
	crStrcat( system_path, lib );
	dll = crDLLOpen( system_path, 1 /*resolveGlobal*/ );
	return dll;
}


static CRDLL *
__findSystemGL( const char *provided_system_path )
{
	return __findSystemLib( provided_system_path, SYSTEM_GL );
}

static SPUGenericFunction
findExtFunction( const crOpenGLInterface *interface, const char *funcName )
{
	/* XXX for some reason, the NVIDIA glXGetProcAddressARB() function
	 * returns pointers that cause Chromium to crash.  If we use the
	 * pointer returned by crDLLGetNoError() instead, we're OK.
	 */
	SPUGenericFunction f = crDLLGetNoError(glDll, funcName);
	if (f)
		return f;
	else if (interface->glXGetProcAddressARB)
		return interface->glXGetProcAddressARB( (const GLubyte *) funcName );
	else
		return NULL;
}
"""


def IsExtensionFunc(func_name):
	"""Determine if the named function is a core function, or extension."""
	cat = apiutil.Category(func_name)
	if cat == "1.0" or cat == "1.1" or cat == "1.2" or cat == "1.3":
		return 0
	else:
		return 1

#
# Generate a no-op function.
#
def GenerateNop(func_name):
	return_type = apiutil.ReturnType(func_name);
	params = apiutil.Parameters(func_name)
	print 'static %s GLLOADER_APIENTRY Nop%s(%s)' % (return_type, func_name, apiutil.MakeDeclarationString(params))
	print '{'
	for (name, type, vecSize) in params:
		if name != "":
			print '\t(void) %s;' % name
	if apiutil.ReturnType(func_name) != 'void':
		print '\treturn 0;'
	print '}'
	print ''



#
# Make no-op funcs for all OpenGL extension functions
#
for func_name in keys:
	if IsExtensionFunc(func_name):
		GenerateNop(func_name)


#
# Generate the crLoadOpenGL() function
#
print """
void
crUnloadOpenGL( void )
{
	crDLLClose( glDll );
	glDll = NULL;
}

/*
 * Initialize the 'interface' structure with the GLX window system
 * interface functions.
 * Then, fill in the table with (name, pointer) pairs for all the core
 * OpenGL entrypoint functions.  But only if table is not NULL
 * Return: number of entries placed in table[], or 0 if error.
 */
int
crLoadOpenGL( crOpenGLInterface *interface, SPUNamedFunctionTable table[] )
{
	static const char *coreFunctions[] = {
"""

for func_name in keys:
	if not IsExtensionFunc(func_name):
		print '\t\t"gl%s",' % func_name

print """
		NULL
	};
	SPUNamedFunctionTable *entry = table;
	int i;

	const char *env_syspath = crGetenv( "CR_SYSTEM_GL_PATH" );
	
	crDebug( "Looking for the system's OpenGL library..." );
	glDll = __findSystemGL( env_syspath );
	if (!glDll)
	{
		crError("Unable to find system OpenGL!");
		return 0;
	}
	
	crDebug( "Found it in %s.", !env_syspath ? "default path" : env_syspath );

"""

useful_glx_functions = [
	"glXGetConfig",
	"glXQueryExtension",
	"glXQueryVersion",
	"glXQueryExtensionsString",
	"glXChooseVisual",
	"glXCreateContext",
	"glXDestroyContext",
	"glXUseXFont",
	"glXIsDirect",
	"glXMakeCurrent",
	"glGetString",
	"glXSwapBuffers",
	"glXGetCurrentDisplay",
	"glXGetCurrentContext",
	"glXGetClientString",
	"glXWaitGL",
	"glXWaitX",
	"glXCopyContext"
]
possibly_useful_glx_functions = [
	"glXGetProcAddressARB",
	"glXJoinSwapGroupNV",
	"glXBindSwapBarrierNV",
	"glXQuerySwapGroupNV",
	"glXQueryMaxSwapGroupsNV",
	"glXQueryFrameCountNV",
	"glXResetFrameCountNV",
	"glXChooseFBConfig",
	"glXGetFBConfigs",
	"glXGetFBConfigAttrib",
	"glXGetVisualFromFBConfig",
	"glXCreateNewContext",
	"glXCreatePbuffer",
	"glXDestroyPbuffer",
	"glXQueryContext",
	"glXQueryDrawable",
	"glXMakeContextCurrent"
]

print '\t/* GLX */'

# XXX merge these loops?
for fun in useful_glx_functions:
	print '\tinterface->%s = (%sFunc_t) crDLLGetNoError( glDll, "%s" );' % (fun, fun, fun)
for fun in possibly_useful_glx_functions:
	print '\tinterface->%s = (%sFunc_t) crDLLGetNoError( glDll, "%s" );' % (fun, fun, fun)

print """
	if (!entry)
		return 1; /* token value */
		
	for (i = 0; coreFunctions[i]; i++) {
		const char *name = coreFunctions[i];
		if (fillin(entry, name + 2, crDLLGetNoError(glDll, name)))
			entry++;
	}

	/* end of table markers */
	entry->name = NULL;
	entry->fn = NULL;
	return entry - table;  /* number of entries filled */
}


/*
 * Fill in table[] with all the OpenGL extension functions that we're
 * interested in.
 */
int
crLoadOpenGLExtensions( const crOpenGLInterface *interface, SPUNamedFunctionTable table[] )
{
	struct extfunc {
		const char *funcName;
		const char *aliasName;
		SPUGenericFunction nopFunction;
	};
	static const struct extfunc functions[] = {
"""

for func_name in keys:
	if IsExtensionFunc(func_name):
		if apiutil.Category(func_name) == "Chromium":
			prefix = "cr"
		else:
			prefix = "gl"
		s = '\t\t{ "' + prefix + func_name + '", '
		a = apiutil.ReverseAlias(func_name)
		if a:
			s += '"' + prefix + a + '", '
		else:
			s += 'NULL, '
		s += '(SPUGenericFunction) Nop' + func_name + ' },'
		print s

print """
		{ NULL, NULL, NULL}
	};
	const struct extfunc *func;
	SPUNamedFunctionTable *entry = table;

	if (interface->glXGetProcAddressARB == NULL)
		crWarning("Unable to find glXGetProcAddressARB() in system GL library");

	for (func = functions; func->funcName; func++) {
		SPUGenericFunction f = findExtFunction(interface, func->funcName);
		if (!f && func->aliasName) {
			f = findExtFunction(interface, func->aliasName);
		}
		if (!f) {
			f = func->nopFunction;
		}
		(void) fillin(entry, func->funcName + 2 , f);  /* +2 to skip "gl" */
		entry++;
	}

	/* end of list */
	entry->name = NULL;
	entry->fn = NULL;
	return entry - table;  /* number of entries filled */
}
"""


print """

#ifdef USE_OSMESA
int crLoadOSMesa( OSMesaContext (**createContext)( GLenum format, OSMesaContext sharelist ), 
		     GLboolean (**makeCurrent)( OSMesaContext ctx, GLubyte *buffer, 
						GLenum type, GLsizei width, GLsizei height ),
		     void (**destroyContext)( OSMesaContext ctx ))
{
	static CRDLL *osMesaDll = NULL;

	const char *env_syspath = crGetenv( "CR_SYSTEM_GL_PATH" );
	
	crDebug( "Looking for the system's OSMesa library..." );
	osMesaDll = __findSystemLib( env_syspath, "libOSMesa.so" );
	if (!osMesaDll)
	{
		crError("Unable to find system OSMesa!");
		return 0;
	}

	crDebug( "Found it in %s.", !env_syspath ? "default path" : env_syspath );

	*createContext =  (OSMesaContext (*) ( GLenum format, OSMesaContext sharelist ))
		crDLLGetNoError( osMesaDll, "OSMesaCreateContext" );

	*makeCurrent =  (GLboolean (*) ( OSMesaContext ctx, GLubyte *buffer, 
					  GLenum type, GLsizei width, GLsizei height ))
		crDLLGetNoError( osMesaDll, "OSMesaMakeCurrent" );

	*destroyContext =  (void (*) ( OSMesaContext ctx))
		crDLLGetNoError( osMesaDll, "OSMesaDestroyContext" );

	return 1;
}
#endif 

"""