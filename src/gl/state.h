#include "gl.h"

#ifndef GL_STATE_H
#define GL_STATE_H

#include "eval.h"
#include "texture.h"
#include "buffers.h"
#include "queries.h"
#include "light.h"
#include "fog.h"
#include "texenv.h"
#include "shader.h"
#include "vertexattrib.h"
#include "program.h"
#include "fpe.h"
#include "pointsprite.h"

typedef struct _glstack_t glstack_t;
typedef struct _glclientstack_t glclientstack_t;

typedef struct {
    GLboolean line_stipple,
              auto_normal,
              normalize,
              normal_rescale,
              lightning,
              blend,
              color_sum,
              pointsprite,
              texgen_s[MAX_TEX],
              texgen_t[MAX_TEX],
              texgen_r[MAX_TEX],
              texgen_q[MAX_TEX],
              plane[MAX_CLIP_PLANES],
              light[MAX_LIGHT],
              map1_color4,
              map1_index,
              map1_normal,
              map1_texture1,
              map1_texture2,
              map1_texture3,
              map1_texture4,
              map1_vertex3,
              map1_vertex4,
              map2_color4,
              map2_index,
              map2_normal,
              map2_texture1,
              map2_texture2,
              map2_texture3,
              map2_texture4,
              map2_vertex3,
              map2_vertex4;
    GLuint    texture[MAX_TEX]; // flag
} enable_state_t;

typedef struct {
    GLenum S;
    GLenum T;
    GLenum R;
    GLenum Q;
    GLfloat S_E[4]; // Eye Plane
    GLfloat T_E[4];
    GLfloat R_E[4];
    GLfloat Q_E[4];
    GLfloat S_O[4]; // Object Plane
    GLfloat T_O[4];
    GLfloat R_O[4];
    GLfloat Q_O[4];
} texgen_state_t;

typedef struct {
    texenv_t        env;
    texfilter_t     filter;
} texenv_state_t;

typedef struct {
    GLuint unpack_row_length,
           unpack_skip_pixels,
           unpack_skip_rows,
           unpack_image_height;
    GLboolean unpack_lsb_first;
    // TODO: use those values
    GLuint pack_row_length,
           pack_skip_pixels,
           pack_skip_rows,
           pack_image_height;
    GLuint  pack_align,
            unpack_align;            
    GLboolean pack_lsb_first;
    gltexture_t *bound[MAX_TEX][ENABLED_TEXTURE_LAST];
    GLboolean pscoordreplace[MAX_TEX];
    khash_t(tex) *list;
    GLuint active;	// active texture
	GLuint client;	// client active texture
} texture_state_t;

typedef struct {
    renderlist_t *active;
    GLboolean compiling;
    GLboolean pending;
    GLboolean begin;
    GLboolean locked;
    GLuint base;
    GLuint name;
    GLenum mode;

    GLuint count;
    GLuint cap;
} displaylist_state_t;

typedef struct {
    rasterpos_t rPos;
    viewport_t viewport;
    GLfloat raster_scale[4];
    GLfloat raster_bias[4];
    GLfloat raster_zoomx;
    GLfloat raster_zoomy;
    GLint index_shift;
    GLint index_offset;
    int     map_color;
    int     map_i2i_size;
    int     map_i2r_size;
    int     map_i2g_size;
    int     map_i2b_size;
    int     map_i2a_size;
    /*
    int     map_s2s_size;
    int     map_r2r_size;
    int     map_g2g_size;
    int     map_b2b_size;
    int     map_a2a_size;
    */
    GLuint  map_i2i[MAX_MAP_SIZE];
    GLubyte map_i2r[MAX_MAP_SIZE];
    GLubyte map_i2g[MAX_MAP_SIZE];
    GLubyte map_i2b[MAX_MAP_SIZE];
    GLubyte map_i2a[MAX_MAP_SIZE];
    /*
    GLuint  map_s2s[MAX_MAP_SIZE];   
    GLubyte map_r2r[MAX_MAP_SIZE];
    GLubyte map_g2g[MAX_MAP_SIZE];
    GLubyte map_b2b[MAX_MAP_SIZE];
    GLubyte map_a2a[MAX_MAP_SIZE];
    */
} raster_state_t;


typedef struct {
    map_state_t *vertex3,
                *vertex4,
                *index,
                *color4,
                *normal,
                *texture1,
                *texture2,
                *texture3,
                *texture4;
} map_states_t;

typedef struct {
	int		top;
	GLuint	*names;
} namestack_t;

typedef struct {
	GLuint  count;
    GLuint *buffer;
    GLuint  size;
    GLfloat zmin;
    GLfloat zmax;
    GLfloat zminoverall;
    GLfloat zmaxoverall;
    GLuint  overflow;
    GLuint  pos;
    GLboolean  hit;
} selectbuf_t;

typedef struct {
	int		top;
    int     identity;
	GLfloat	*stack;
} matrixstack_t;

typedef enum {
    ENABLED_ALPHA,
    ENABLED_BLEND,
    ENABLED_CULL,
    ENABLED_DEPTH,
    ENABLED_TEX2D_TEX0,
    ENABLED_TEX2D_TEX1,
    ENABLED_TEX2D_TEX2,
    ENABLED_TEX2D_TEX3,
    ENABLED_TEX2D_TEX4,
    ENABLED_TEX2D_TEX5,
    ENABLED_TEX2D_TEX6,
    ENABLED_TEX2D_TEX7,
    ENABLED_LAST
} statbatch_enabled_t;

typedef struct {
    int    active_tex_changed;
    GLenum active_tex;  // current active texture
    GLenum bound_targ[MAX_TEX];
    GLenum bound_tex[MAX_TEX];
    int enabled[ENABLED_LAST];    // the enabled are: 0=not set, 1=enabled, 2=disabled
    GLenum blendfunc_s;
    GLenum blendfunc_d;
} statebatch_t;

typedef struct {
    GLboolean   vertex_array,
                color_array,
                normal_array,
                tex_coord_array[MAX_TEX];
} clientstate_t;

typedef struct {
    khash_t(shaderlist)    *shaders;
    khash_t(programlist)   *programs;
    GLuint                 program;
    program_t              *glprogram;
    int                    es2; // context is es2
} glsl_t;

typedef struct {
    vertexattrib_t  vertexattrib[MAX_VATTRIB];
    vertexattrib_t  wanted[MAX_VATTRIB];
    GLuint          program;
    program_t       *glprogram;
} gleshard_t;

typedef struct {
    GLuint          vertexshader;
    GLuint          pixelshader;
    GLuint          vertexshader_alpha;
    GLuint          pixelshader_alpha;
    GLuint          program;
    GLuint          program_alpha;
    GLfloat         vert[8], tex[8];
} glesblit_t;

typedef struct {
    GLboolean  secondary_array,
    color_array,
    normal_array,
    vertex_array,
    tex_coord_array[MAX_TEX];

    GLuint          client; //active client texture

    pointer_state_t vert;
    pointer_state_t normal;
    pointer_state_t color;
    pointer_state_t secondary;
    pointer_state_t tex[MAX_TEX];
} fpestatus_t;

typedef struct {
    int                 dummy[16];  // dummy zone, test for memory overwriting...
    displaylist_state_t list;
    enable_state_t      enable;
    map_state_t         *map_grid;
    map_states_t        map1, map2;
    khash_t(gllisthead) *headlists;
    texgen_state_t      texgen[MAX_TEX];
    texenv_state_t      texenv[MAX_TEX];
    texture_state_t     texture;
    GLfloat             vertex[4];
    GLfloat             color[4];
    GLfloat             secondary[4];
    GLfloat             texcoord[MAX_TEX][4];
    GLfloat             normal[3];
    int	                render_mode;
    int                 polygon_mode;
    namestack_t         namestack;
    GLfloat             mvp_matrix[16];
    int                 mvp_matrix_dirty;
    matrixstack_t       *modelview_matrix;
    matrixstack_t       *projection_matrix;
    matrixstack_t       **texture_matrix;
    int                 matrix_mode;
    selectbuf_t         selectbuf;
    khash_t(glvao)      *vaos;
    khash_t(buff)       *buffers;
    glvao_t             *vao;
    glbuffer_t          *defaultvbo; 
    glvao_t             *defaultvao;
    int                 shim_error;
    GLenum              last_error;
    GLuint              gl_batch;
    int                 init_batch;
    GLint               vp[4];
    statebatch_t        statebatch;
    clientstate_t       clientstate;
    khash_t(queries)    *queries;
    glstack_t           *stack;
    glclientstack_t     *clientStack;
    raster_state_t      raster;
    int                 emulatedPixmap;
    int                 emulatedWin;
    int                 shared_cnt;
    light_state_t       light;
    fog_t               fog;
    material_state_t    material;
    float               planes[MAX_CLIP_PLANES][4];
    pointsprite_t       pointsprite;
    int                 immediateMV;
    GLenum              shademodel;
    GLenum              alphafunc;
    GLfloat             alpharef;
    GLenum              logicop;
    glsl_t              glsl;
    fpe_state_t         *fpe_state;
    fpe_fpe_t           *fpe;
    fpestatus_t         fpe_client;
    gleshard_t          gleshard;
    glesblit_t          *blit;
} glstate_t;

#endif
