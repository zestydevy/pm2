#ifdef _cplusplus
extern "C"{
#endif

// Surface Types
#define SURFACE_DEFAULT                    0x0000 // Environment default
#define SURFACE_WATER                      0x0001
#define SURFACE_BORDER                     0x0004

#define TERRAIN_LOAD_VERTICES    0x0040
#define TERRAIN_LOAD_CONTINUE    0x0041
#define TERRAIN_LOAD_END         0x0042
#define TERRAIN_LOAD_OBJECTS     0x0043
#define TERRAIN_LOAD_ENVIRONMENT 0x0044

// Collision Data Routine Initiate
#define COL_INIT() TERRAIN_LOAD_VERTICES

// Collision Vertices Read Initiate
#define COL_VERTEX_INIT(vtxNum) vtxNum

// Collision Vertex
#define COL_VERTEX(x, y, z) x, y, z

// Collision Tris Initiate
#define COL_TRI_INIT(surfType, triNum) surfType, triNum

// Collision Tri
#define COL_TRI(v1, v2, v3) v1, v2, v3

// Collision Tri With Special Params
#define COL_TRI_SPECIAL(v1, v2, v3, param) v1, v2, v3, param

// Collision Tris Stop Loading
#define COL_TRI_STOP() TERRAIN_LOAD_CONTINUE

// End Collision Data
#define COL_END() TERRAIN_LOAD_END

typedef struct t_collision{
    short code;
} Collision;

#ifdef _cplusplus
}
#endif