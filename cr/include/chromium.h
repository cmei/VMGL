/* Copyright (c) 2001, Stanford University
 * All rights reserved.
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

/**
 * Public Chromium exports.
 * Parallel Chromium applications will include this header.
 */


#ifndef __CHROMIUM_H__
#define __CHROMIUM_H__


/**********************************************************************/
/*****             System includes and other cruft                *****/
/**********************************************************************/

#include "cr_compiler.h"

/*
 * We effectively wrap gl.h, glu.h, etc, just like GLUT
 */

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#define GLX

#include <GL/gl.h>
#include <GL/glu.h>


#ifdef GLX
#ifndef GLX_GLXEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#endif
#include <GL/glx.h>
#endif

#ifdef USE_OSMESA
#include <GL/osmesa.h>
#endif

#if !defined(FreeBSD)
#include <malloc.h>  /* to get ptrdiff_t used below */
#endif

#include <GL/glext.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APIENTRY
#define APIENTRY
#endif


/**********************************************************************/
/*****     Define things that might have been missing in gl.h     *****/
/**********************************************************************/

/* 
 * Define missing GLX tokens:
 */

#ifndef GLX_SAMPLE_BUFFERS_SGIS
#define GLX_SAMPLE_BUFFERS_SGIS    0x186a0 /*100000*/
#endif
#ifndef GLX_SAMPLES_SGIS
#define GLX_SAMPLES_SGIS           0x186a1 /*100001*/
#endif
#ifndef GLX_VISUAL_CAVEAT_EXT
#define GLX_VISUAL_CAVEAT_EXT       0x20  /* visual_rating extension type */
#endif

/*
 * Define missing 1.2 tokens:
 */
#ifndef GL_SMOOTH_POINT_SIZE_RANGE
#define GL_SMOOTH_POINT_SIZE_RANGE		0x0B12
#endif

#ifndef GL_SMOOTH_POINT_SIZE_GRANULARITY
#define GL_SMOOTH_POINT_SIZE_GRANULARITY	0x0B13
#endif

#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE		0x0B22
#endif

#ifndef GL_SMOOTH_LINE_WIDTH_GRANULARITY
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY	0x0B23
#endif

#ifndef GL_ALIASED_POINT_SIZE_RANGE
#define GL_ALIASED_POINT_SIZE_RANGE		0x846D
#endif

#ifndef GL_ALIASED_LINE_WIDTH_RANGE
#define GL_ALIASED_LINE_WIDTH_RANGE		0x846E
#endif

#ifndef GL_COLOR_MATRIX_STACK_DEPTH
#define GL_COLOR_MATRIX_STACK_DEPTH		0x80B2
#endif

#ifndef GL_COLOR_MATRIX
#define GL_COLOR_MATRIX				0x80B1
#endif

#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D				0x806F
#endif

#ifndef GL_MAX_3D_TEXTURE_SIZE
#define GL_MAX_3D_TEXTURE_SIZE			0x8073
#endif

#ifndef GL_PACK_SKIP_IMAGES
#define GL_PACK_SKIP_IMAGES			0x806B
#endif

#ifndef GL_PACK_IMAGE_HEIGHT
#define GL_PACK_IMAGE_HEIGHT			0x806C
#endif

#ifndef GL_UNPACK_SKIP_IMAGES
#define GL_UNPACK_SKIP_IMAGES			0x806D
#endif

#ifndef GL_UNPACK_IMAGE_HEIGHT
#define GL_UNPACK_IMAGE_HEIGHT			0x806E
#endif

#ifndef GL_PROXY_TEXTURE_3D
#define GL_PROXY_TEXTURE_3D			0x8070
#endif

#ifndef GL_TEXTURE_DEPTH
#define GL_TEXTURE_DEPTH			0x8071
#endif

#ifndef GL_TEXTURE_WRAP_R
#define GL_TEXTURE_WRAP_R			0x8072
#endif

#ifndef GL_TEXTURE_BINDING_3D
#define GL_TEXTURE_BINDING_3D			0x806A
#endif

#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES		0x80E8
#endif

#ifndef GL_MAX_ELEMENTS_INDICES
#define GL_MAX_ELEMENTS_INDICES			0x80E9
#endif


/*
 * Define missing ARB_imaging tokens
 */

#ifndef GL_BLEND_EQUATION
#define GL_BLEND_EQUATION			0x8009
#endif

#ifndef GL_MIN
#define GL_MIN					0x8007
#endif

#ifndef GL_MAX
#define GL_MAX					0x8008
#endif

#ifndef GL_FUNC_ADD
#define GL_FUNC_ADD				0x8006
#endif

#ifndef GL_FUNC_SUBTRACT
#define GL_FUNC_SUBTRACT			0x800A
#endif

#ifndef GL_FUNC_REVERSE_SUBTRACT
#define GL_FUNC_REVERSE_SUBTRACT		0x800B
#endif

#ifndef GL_BLEND_COLOR
#define GL_BLEND_COLOR				0x8005
#endif

#ifndef GL_PER_STAGE_CONSTANTS_NV
#define GL_PER_STAGE_CONSTANTS_NV 		0x8535
#endif

#ifndef GL_FOG_COORDINATE_ARRAY_POINTER_EXT
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT     0x8456
#endif

typedef void (*CR_GLXFuncPtr)();
#ifndef GLX_ARB_get_proc_address
#define GLX_ARB_get_proc_address 1
CR_GLXFuncPtr glXGetProcAddressARB( const GLubyte *name );
#endif /* GLX_ARB_get_proc_address */

#ifndef GLX_VERSION_1_4
CR_GLXFuncPtr glXGetProcAddress( const GLubyte *name );
#endif /* GLX_ARB_get_proc_address */

#ifndef GL_RASTER_POSITION_UNCLIPPED_IBM
#define GL_RASTER_POSITION_UNCLIPPED_IBM  0x19262
#endif

#ifndef GL_VERSION_1_5

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

/* prototype these functions for vmgl_lib/getprocaddress.c */
extern void APIENTRY glGenQueries(GLsizei n, GLuint *ids);
extern void APIENTRY glDeleteQueries(GLsizei n, const GLuint *ids);
extern GLboolean APIENTRY glIsQuery(GLuint id);
extern void APIENTRY glBeginQuery(GLenum target, GLuint id);
extern void APIENTRY glEndQuery(GLenum target);
extern void APIENTRY glGetQueryiv(GLenum target, GLenum pname, GLint *params);
extern void APIENTRY glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params);
extern void APIENTRY glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params);
extern void APIENTRY glBindBuffer(GLenum, GLuint);
extern void APIENTRY glDeleteBuffers(GLsizei, const GLuint *);
extern void APIENTRY glGenBuffers(GLsizei, GLuint *);
extern GLboolean APIENTRY glIsBuffer(GLuint);
extern void APIENTRY glBufferData(GLenum, GLsizeiptr, const GLvoid *, GLenum);
extern void APIENTRY glBufferSubData(GLenum, GLintptr, GLsizeiptr, const GLvoid *);
extern void APIENTRY glGetBufferSubData(GLenum, GLintptr, GLsizeiptr, GLvoid *);
extern GLvoid* APIENTRY glMapBuffer(GLenum, GLenum);
extern GLboolean APIENTRY glUnmapBuffer(GLenum);
extern void APIENTRY glGetBufferParameteriv(GLenum, GLenum, GLint *);
extern void APIENTRY glGetBufferPointerv(GLenum, GLenum, GLvoid* *);

#endif


/**********************************************************************/
/*****            Chromium Extensions to OpenGL                   *****/
/*****                                                            *****/
/***** Chromium owns the OpenGL enum range 0x8AF0-0x8B2F          *****/
/**********************************************************************/

#ifndef GL_CR_synchronization
#define GL_CR_synchronization 1

typedef void (APIENTRY *glBarrierCreateCRProc) (GLuint name, GLuint count);
typedef void (APIENTRY *glBarrierDestroyCRProc) (GLuint name);
typedef void (APIENTRY *glBarrierExecCRProc) (GLuint name);
typedef void (APIENTRY *glSemaphoreCreateCRProc) (GLuint name, GLuint count);
typedef void (APIENTRY *glSemaphoreDestroyCRProc) (GLuint name);
typedef void (APIENTRY *glSemaphorePCRProc) (GLuint name);
typedef void (APIENTRY *glSemaphoreVCRProc) (GLuint name);

extern void APIENTRY glBarrierCreateCR(GLuint name, GLuint count);
extern void APIENTRY glBarrierDestroyCR(GLuint name);
extern void APIENTRY glBarrierExecCR(GLuint name);
extern void APIENTRY glSemaphoreCreateCR(GLuint name, GLuint count);
extern void APIENTRY glSemaphoreDestroyCR(GLuint name);
extern void APIENTRY glSemaphorePCR(GLuint name);
extern void APIENTRY glSemaphoreVCR(GLuint name);

#endif /* GL_CR_synchronization */


#ifndef GL_CR_bounds_info
#define GL_CR_bounds_info 1
/* Private, internal Chromium function */
/*
typedef void (APIENTRY *glBoundsInfoCRProc)(const CRrecti *, const GLbyte *, GLint, GLint);
*/
#endif /* GL_CR_bounds_info */


#ifndef GL_CR_state_parameter
#define GL_CR_state_parameter 1

typedef void (APIENTRY *glChromiumParameteriCRProc) (GLenum target, GLint value);
typedef void (APIENTRY *glChromiumParameterfCRProc) (GLenum target, GLfloat value);
typedef void (APIENTRY *glChromiumParametervCRProc) (GLenum target, GLenum type, GLsizei count, const GLvoid *values);
typedef void (APIENTRY *glGetChromiumParametervCRProc) (GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values);

extern void APIENTRY glChromiumParameteriCR(GLenum target, GLint value);
extern void APIENTRY glChromiumParameterfCR(GLenum target, GLfloat value);
extern void APIENTRY glChromiumParametervCR(GLenum target, GLenum type, GLsizei count, const GLvoid *values);
extern void APIENTRY glGetChromiumParametervCR(GLenum target, GLuint index, GLenum type, GLsizei count, GLvoid *values);


#endif /* GL_CR_state_parameter */


#ifndef GL_CR_cursor_position
#define GL_CR_cursor_position 1
/* For virtual cursor feature (show_cursor) */

#define GL_CURSOR_POSITION_CR  0x8AF0

#endif /* GL_CR_cursor_position */


#ifndef GL_CR_bounding_box
#define GL_CR_bounding_box 1
/* To set bounding box from client app */

#define GL_DEFAULT_BBOX_CR	0x8AF1
#define GL_SCREEN_BBOX_CR	0x8AF2
#define GL_OBJECT_BBOX_CR	0x8AF3

#endif /* GL_CR_bounding_box */


#ifndef GL_CR_print_string
#define GL_CR_print_string 1
/* To print a string to stdout */
#define GL_PRINT_STRING_CR	0x8AF4

#endif /* GL_CR_print_string */


#ifndef GL_CR_tilesort_info
#define GL_CR_tilesort_info 1
/* To query tilesort information */

#define GL_MURAL_SIZE_CR             0x8AF5
#define GL_NUM_SERVERS_CR            0x8AF6
#define GL_NUM_TILES_CR              0x8AF7
#define GL_TILE_BOUNDS_CR            0x8AF8
#define GL_VERTEX_COUNTS_CR          0x8AF9
#define GL_RESET_VERTEX_COUNTERS_CR  0x8AFA
#define GL_SET_MAX_VIEWPORT_CR       0x8AFB

#endif /* GL_CR_tilesort_info */


#ifndef GL_CR_head_spu_name
#define GL_CR_head_spu_name 1
/* To fetch name of first SPU on a node */

#define GL_HEAD_SPU_NAME_CR         0x8AFC

#endif /* GL_CR_head_spu_name */


#ifndef GL_CR_performance_info
#define GL_CR_performance_info 1
/* For gathering performance metrics */

#define GL_PERF_GET_FRAME_DATA_CR       0x8AFD
#define GL_PERF_GET_TIMER_DATA_CR       0x8AFE
#define GL_PERF_DUMP_COUNTERS_CR        0x8AFF
#define GL_PERF_SET_TOKEN_CR            0x8B00
#define GL_PERF_SET_DUMP_ON_SWAP_CR     0x8B01
#define GL_PERF_SET_DUMP_ON_FINISH_CR   0x8B02
#define GL_PERF_SET_DUMP_ON_FLUSH_CR    0x8B03
#define GL_PERF_START_TIMER_CR          0x8B04
#define GL_PERF_STOP_TIMER_CR           0x8B05

#endif /* GL_CR_performance_info */


#ifndef GL_CR_window_size
#define GL_CR_window_size 1
/* To communicate window size changes */

#define GL_WINDOW_SIZE_CR               0x8B06
#define GL_MAX_WINDOW_SIZE_CR           0x8B24  /* new */

#endif /* GL_CR_window_size */


#ifndef GL_CR_tile_info
#define GL_CR_tile_info 1
/* To send new tile information to a server */

#define GL_TILE_INFO_CR                 0x8B07

#endif /* GL_CR_tile_info */


#ifndef GL_CR_gather
#define GL_CR_gather 1
/* For aggregate transfers  */

#define GL_GATHER_DRAWPIXELS_CR         0x8B08
#define GL_GATHER_PACK_CR               0x8B09
#define GL_GATHER_CONNECT_CR            0x8B0A
#define GL_GATHER_POST_SWAPBUFFERS_CR   0x8B0B

#endif /* GL_CR_gather */


#ifndef GL_CR_saveframe
#define GL_CR_saveframe 1

#define GL_SAVEFRAME_ENABLED_CR         0x8B0C
#define GL_SAVEFRAME_FRAMENUM_CR        0x8B0D
#define GL_SAVEFRAME_STRIDE_CR          0x8B0E
#define GL_SAVEFRAME_SINGLE_CR          0x8B0F
#define GL_SAVEFRAME_FILESPEC_CR        0x8B10

#endif /* GL_CR_saveframe */


#ifndef GL_CR_readback_barrier_size
#define GL_CR_readback_barrier_size 1

#define GL_READBACK_BARRIER_SIZE_CR     0x8B11

#endif /* GL_CR_readback_barrier_size */


#ifndef GL_CR_server_id_sharing
#define GL_CR_server_id_sharing 1

#define GL_SHARED_DISPLAY_LISTS_CR      0x8B12
#define GL_SHARED_TEXTURE_OBJECTS_CR    0x8B13
#define GL_SHARED_PROGRAMS_CR           0x8B14

#endif /* GL_CR_server_id_sharing */


#ifndef GL_CR_server_matrix
#define GL_CR_server_matrix 1

#define GL_SERVER_VIEW_MATRIX_CR        0x8B15
#define GL_SERVER_PROJECTION_MATRIX_CR  0x8B16
#define GL_SERVER_FRUSTUM_CR            0x8B17
#define GL_SERVER_CURRENT_EYE_CR        0x8B18

#endif /* GL_CR_server_matrix */


#ifndef GL_CR_window_position
#define GL_CR_window_position 1

#define GL_WINDOW_POSITION_CR           0x8B19

#endif /* GL_CR_window_position */


#ifndef GL_CR_zpix
#define GL_CR_zpix 1

#define GL_ZLIB_COMPRESSION_CR          0x8B20
#define GL_RLE_COMPRESSION_CR           0x8B21
#define GL_PLE_COMPRESSION_CR           0x8B22

/* Two new opcodes for vmgl. For passing data between GL stub and render spu */
#define GL_WINDOW_XID_CR		0x8B23
#define GL_VNCWIN_CR			0x8B24

/* XXX A better name would be glCompressedDrawPixelsCR() */
extern void APIENTRY glZPixCR(GLsizei width, GLsizei height, GLenum format,
                              GLenum type, GLenum compressionType,
                              GLint client, GLint compressedSize,
                              const GLvoid *image);

#endif /* GL_CR_zpix */


/**********************************************************************/
/*****                Chromium-specific API                       *****/
/**********************************************************************/


/*
 * Accepted by crCreateContext() and crCreateWindow() visBits parameter.
 * Used to communicate visual attributes throughout Chromium.
 */
#define CR_RGB_BIT            0x1
#define CR_ALPHA_BIT          0x2
#define CR_DEPTH_BIT          0x4
#define CR_STENCIL_BIT        0x8
#define CR_ACCUM_BIT          0x10
#define CR_DOUBLE_BIT         0x20
#define CR_STEREO_BIT         0x40
#define CR_MULTISAMPLE_BIT    0x80
#define CR_OVERLAY_BIT        0x100
#define CR_PBUFFER_BIT        0x200
#define CR_ALL_BITS           0x3ff


/* Accepted by crSwapBuffers() flag parameter */
#define CR_SUPPRESS_SWAP_BIT 0x1


typedef GLint (APIENTRY *crCreateContextProc)(const char *dpyName, GLint visBits);
typedef void (APIENTRY *crDestroyContextProc)(GLint context);
typedef void (APIENTRY *crMakeCurrentProc)(GLint window, GLint context);
typedef GLint (APIENTRY *crGetCurrentContextProc)(void);
typedef GLint (APIENTRY *crGetCurrentWindowProc)(void);
typedef void (APIENTRY *crSwapBuffersProc)(GLint window, GLint flags);

typedef GLint (APIENTRY *crWindowCreateProc)(const char *dpyName, GLint visBits);
typedef void (APIENTRY *crWindowDestroyProc)(GLint window);
typedef void (APIENTRY *crWindowSizeProc)(GLint window, GLint w, GLint h);
typedef void (APIENTRY *crWindowPositionProc)(GLint window, GLint x, GLint y);
typedef void (APIENTRY *crWindowShowProc)( GLint window, GLint flag );

extern GLint APIENTRY crCreateContext(const char *dpyName, GLint visBits);
extern void APIENTRY crDestroyContext(GLint context);
extern void APIENTRY crMakeCurrent(GLint window, GLint context);
extern GLint APIENTRY crGetCurrentContext(void);
extern GLint APIENTRY crGetCurrentWindow(void);
extern void APIENTRY crSwapBuffers(GLint window, GLint flags);
extern GLint APIENTRY crWindowCreate(const char *dpyName, GLint visBits);
extern void APIENTRY crWindowDestroy(GLint window);
extern void APIENTRY crWindowSize(GLint window, GLint w, GLint h);
extern void APIENTRY crWindowPosition(GLint window, GLint x, GLint y);
extern void APIENTRY crWindowShow( GLint window, GLint flag );

typedef int (CR_APIENTRY *CR_PROC)();
CR_PROC APIENTRY crGetProcAddress( const char *name );



/**********************************************************************/
/*****                 Other useful stuff                         *****/
/**********************************************************************/

#if defined(GLX_ARB_get_proc_address)
#define GET_PROC(NAME) glXGetProcAddressARB((const GLubyte *) (NAME))
#else
/* For SGI, etc that don't have glXGetProcAddress(). */
#define GET_PROC(NAME) NULL
#endif


#ifdef __cplusplus
}
#endif

#endif /* __CHROMIUM_H__ */
