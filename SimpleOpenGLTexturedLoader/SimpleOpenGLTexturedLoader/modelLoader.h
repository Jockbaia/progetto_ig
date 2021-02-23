#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "GL/glut.h"
#include <IL/il.h>
#include <string.h>
#include <map>
#pragma once
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)
#define TRUE                1
#define FALSE               0

///////////////////////
/* VARIABILI GLOBALI */
///////////////////////

// Le variabili sono dichiarate in modelLoader.cpp

// Global Scene Object Assimp
extern const struct aiScene* scene;
extern GLuint scene_list;
extern struct aiVector3D scene_min, scene_max, scene_center;

// Immagini e texture
extern std::map<std::string, GLuint*> textureIdMap;		// Map image filenames to textureIds
extern GLuint* textureIds;								// Pointer to texture Array
static const std::string basepath = "./models/";		// Per i file blend

//	Currently this is hardcoded
//	Static const std::string basepath = "./models/textures/"; //obj..

//////////////
/* FUNZIONI */
//////////////

void reshape(int width, int height);
void get_bounding_box_for_node(const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);
void get_bounding_box(struct aiVector3D* min, struct aiVector3D* max);
void color4_to_float4(const struct aiColor4D* c, float f[4]);
void set_float4(float f[4], float a, float b, float c, float d);
void apply_material(const struct aiMaterial* mtl);
void Color4f(const struct aiColor4D* color);
void recursive_render(const struct aiScene* sc, const struct aiNode* nd, float scale);

int loadasset(const char* path);
int LoadGLTextures(const aiScene* scene);
int InitGL();