// assimp include files. These three are usually needed.
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include <iostream>
#include "GL/glut.h"
#include <IL/il.h>
#include <irrKlang.h>

//to map image filenames to textureIds
#include <string.h>
#include <map>

//included in the original project
#include <stdlib.h>
#include <Windows.h>

#define XRES 1280
#define YRES 720

//				*** GAME VARIABLES ***
char menu = 1;
char difficulty = 2;	// 1 = EASY, 2 = MEDIUM, 3 = HARD
void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;		//font schermata menu
void* font2 = GLUT_BITMAP_HELVETICA_18;
short int pos_titolo, pos_start, pos_diff;

//				***	SONG(S)	***
typedef struct n {
	char key;
	float x;
	struct n* next = NULL;
} nota_t;

typedef struct s {
	//std::string name = "Twinkle twinkle little star";
	float base_coord = 1;
	float offset = 0.1;
	nota_t *note = NULL;
} song_t;

nota_t DO, RE, MI, FA, SOL, LA, SI, pause;
nota_t vett[50];
int n = 0;
float speed = -0.01;
song_t ttls;

using namespace irrklang;
ISoundEngine* piano = createIrrKlangDevice();

// currently this is hardcoded
//static const std::string basepath = "./models/textures/"; //obj..
static const std::string basepath = "./models/"; //per i file blend

// the global Assimp scene object
const struct aiScene* scene = NULL;
GLuint scene_list = 0;
struct aiVector3D scene_min, scene_max, scene_center;

// angle of rotation for the camera direction

float angle_start = 28.3f;

// COORDINATE MOVIMENTO
// XZ position of the camera
// actual vector representing the camera's direction
float lx = 0.00433532, lz = 0.999991;
float x = -6.86088, z = 6.1202;
float angle = 28.27f;
float height = 3.f;

// current rotation angle
// static float angle = 28;

// images / texture
std::map<std::string, GLuint*> textureIdMap;	// map image filenames to textureIds
GLuint* textureIds;							// pointer to texture Array

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 15.0f, 1.0f };

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)
#define TRUE                1
#define FALSE               0

// MENU COORDS

double eyex = 0.f;
double eyey = 0.f;
double eyez = -3.f;
double centerx = 0.f;
double centery = 0.f;
double centerz = 5.f;
double upx = 0.f;
double upy = 1.f;
double upz = 0.f;

// ----------------------------------------------------------------------------
void reshape(int width, int height)
{
	const double aspectRatio = (float)width / height, fieldOfView = 45.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, aspectRatio,
		1.0, 1000.0);  // Znear and Zfar 
}

// ----------------------------------------------------------------------------
void get_bounding_box_for_node(const struct aiNode* nd,
	struct aiVector3D* min,
	struct aiVector3D* max,
	struct aiMatrix4x4* trafo
) {
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------

void get_bounding_box(struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode, min, max, &trafo);
}

// ----------------------------------------------------------------------------

void color4_to_float4(const struct aiColor4D* c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// ----------------------------------------------------------------------------

void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
void apply_material(const struct aiMaterial* mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	int max;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture
	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, (unsigned int*)&max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, (unsigned int*)&max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, (unsigned int*)&max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, (unsigned int*)&max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

// ----------------------------------------------------------------------------

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
void Color4f(const struct aiColor4D* color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

// ----------------------------------------------------------------------------

void recursive_render(const struct aiScene* sc, const struct aiNode* nd, float scale)
{
	unsigned int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	printf("Node name: %s\n", nd->mName.data);

	//m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		///
		printf("Drawing MESH with this name: %s\n", mesh->mName.data);

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);


		if (mesh->HasTextureCoords(0))
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		if (mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if (mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}



		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices)
			{
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for (i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if (mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
				if (mesh->mNormals != NULL)

					if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();

		}

	}


	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursive_render(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}


// ----------------------------------------------------------------------------

void do_motion(void)
{
	//static GLint prev_time = 0;
	static float increment = 0.05;
	//int time = glutGet(GLUT_ELAPSED_TIME);
	if (angle_start > 40) {
		increment = -0.05;
	}
	if (angle_start < 28) {
		increment = +0.05;
	}

	angle_start += increment;
	//prev_time = time;

	glutPostRedisplay();
}

// --------------------- GAME SECTION ----------------------------

void output(int x, int y, std::string str, void* font)
{
	int len, i;

	glRasterPos2f(x, y);        //set text position in the window
	len = str.length();

	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glutPostRedisplay();
}

void show_room(void) {
	float tmp;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	// rotate it around the y axis
	glRotatef(angle_start, 0.f, 1.f, 0.f);
	
	// scale the whole asset to fit into our view frustum 
	// tmp = scene_max.x - scene_min.x;
	// tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	// tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	// tmp = 1.f / tmp;
	// glScalef(tmp, tmp, tmp);
	glScalef(3, 3, 3);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	// if the display list has not been made yet, create a new one and
	// fill it with scene contents
	if (scene_list == 0) {
		scene_list = glGenLists(1);
		glNewList(scene_list, GL_COMPILE);
		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
		recursive_render(scene, scene->mRootNode, 1.0);
		glEndList();
	}

	glCallList(scene_list);

	glutSwapBuffers();

	do_motion();
}

void get_coords(void) {
	std::cout << "gluLookAt(" << x << ", " << height << ", " << z << ", " << x + lx << ", centery, " << x + lz << ", " << upx << ", " << upy << ", " << upz << ");\n"
		<< "X " << x << " | Z " << z << " | LX " << lx << " | LZ " << lz << "| ANGLE " << angle << "\n";
}

void play_song() {
	//glClear(GL_COLOR_BUFFER_BIT);

	glScalef(1, 1, 1);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glLoadIdentity();
	n = 0;
	/*while (n < 48) {
		if (ttls.base_coord + ttls.offset * n > 2)
			break;

		glTranslatef(ttls.note[n].x, ttls.base_coord + n * ttls.offset, -3);
		glutSolidSphere(0.03, 4, 4);
		glTranslatef(-ttls.note[n].x, -ttls.base_coord - n * ttls.offset, 3);
		glPopMatrix();
		n++;
	}*/
	ttls.base_coord = ttls.base_coord + speed;
	while (n < 48) {
		glTranslatef(ttls.note[n].x, -1 + n * ttls.offset, -3);
		glutSolidSphere(0.03, 4, 4);
		glTranslatef(-ttls.note[n].x, +1 - n * ttls.offset, 3);
		glPopMatrix();
		n++;
	}
	glutSwapBuffers();
	//glutPostRedisplay();
}

void show_piano(void) {
	float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// angle = 28.31;

	//gluLookAt(x, height, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	gluLookAt(-6.8f, 0.7f, 1.f, -6.8f, -0.3f, 5, 0, 1, 0);

	// scale the whole asset to fit into our view frustum 
	 tmp = scene_max.x - scene_min.x;
	 tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	 tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	 tmp = 1.f / tmp;
	 //glScalef(tmp, tmp, tmp);

	glScalef(4,4,4);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);

	// if the display list has not been made yet, create a new one and
	// fill it with scene contents
	if (scene_list == 0) {
		scene_list = glGenLists(1);
		glNewList(scene_list, GL_COMPILE);
		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
		recursive_render(scene, scene->mRootNode, 1.0);
		glEndList();
	}

	glCallList(scene_list);

	/*//TEST POSIZIONE PIANO
	//glScalef(1,1,1);
	glColor3f(1.0, 0.0, 1.0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.05,-0.4,-3);
	glutSolidSphere(0.04, 4, 4);
	glPopMatrix();*/

	play_song();
	//glutSwapBuffers();
	//glutPostRedisplay();
}

void menu_display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(1.0, XRES, 1.0, YRES);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	char text_row_3[30] = "";
	std::string text_row_1 = "W E L C O M E";
	std::string text_row_2 = "Start new game";
	sprintf(text_row_3, "Select difficulty: %d", difficulty);
	std::string text_row_4 = "(1=EASY, 2=MEDIUM, 3=HARD)";

	pos_titolo = 500;
	output(500, pos_titolo, text_row_1, font1);
	pos_start = 150;
	output(500, pos_start, text_row_2, font2);
	pos_diff = 100;
	output(495, pos_diff, text_row_3, font2);
	output(440, pos_diff - 50, text_row_4, font2);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	show_room();
	glutSwapBuffers();
}

void define_main_pitches() {
	//do
	DO.key = 'A';
	DO.x = -0.12;
	//re
	RE.key = 'S';
	RE.x = -0.06;
	//mi
	MI.key = 'D';
	MI.x = 0;
	//fa
	FA.key = 'F';
	FA.x = 0.06;
	//sol
	SOL.key = 'G';
	SOL.x = 0.12;
	//la
	LA.key = 'H';
	LA.x = 0.18;
	//si
	SI.key = 'J';
	SI.x = 0.24;
	//pause (fake)
	pause.key = '_';
	pause.x = 1;

}

void create_song() {
	//create ttls
	vett[0] = DO;
	vett[1] = DO;
	vett[2] = SOL;
	vett[3] = SOL;
	vett[4] = LA;
	vett[5] = LA;
	vett[6] = SOL;
	vett[7] = pause;

	vett[8] = FA;
	vett[9] = FA;
	vett[10] = MI;
	vett[11] = MI;
	vett[12] = RE;
	vett[13] = RE;
	vett[14] = DO;
	vett[15] = pause;

	vett[16] = SOL;
	vett[17] = SOL;
	vett[18] = FA;
	vett[19] = FA;
	vett[20] = MI;
	vett[21] = MI;
	vett[22] = RE;
	vett[23] = pause;

	vett[24] = SOL;
	vett[25] = SOL;
	vett[26] = FA;
	vett[27] = FA;
	vett[28] = MI;
	vett[29] = MI;
	vett[30] = RE;
	vett[31] = pause;

	vett[32] = DO;
	vett[33] = DO;
	vett[34] = SOL;
	vett[35] = SOL;
	vett[36] = LA;
	vett[37] = LA;
	vett[38] = SOL;
	vett[39] = pause;

	vett[40] = FA;
	vett[41] = FA;
	vett[42] = MI;
	vett[43] = MI;
	vett[44] = RE;
	vett[45] = RE;
	vett[46] = DO;
	vett[47] = pause;
	ttls.note = vett;
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display according to the flag:
	// 1 = Main Men�
	// 0 = Free-roaming / game

	if (menu == 1) {
		menu_display();
		show_room();
	}
	else {
		show_piano();
//		play_song();
	}
}

// ----------------------------------------------------------------------------

int loadasset(const char* path)
{
	// we are taking one of the postprocessing presets to avoid
	// writing 20 single postprocessing flags here.
	scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);

	if (scene) {
		get_bounding_box(&scene_min, &scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}
	return 1;
}

int LoadGLTextures(const aiScene* scene)
{
	ILboolean success;

	/* Before calling ilInit() version should be checked. */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		ILint test = ilGetInteger(IL_VERSION_NUM);
		/// wrong DevIL version ///
		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
		char* cErr_msg = (char*)err_msg.c_str();

		return -1;
	}

	ilInit(); /* Initialization of DevIL */

	//if (scene->HasTextures()) abortGLInit("Support for meshes with embedded textures is not implemented");

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m = 0; m < scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = textureIdMap.size();

	/* array with DevIL image IDs */
	ILuint* imageIds = NULL;
	imageIds = new ILuint[numTextures];

	/* generate DevIL Image IDs */
	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */

	/* create and fill array with GL texture ids */
	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

	/* define texture path */
	//std::string texturepath = "../../../test/models/Obj/";

	/* get iterator */
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();

	for (int i = 0; i < numTextures; i++)
	{

		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second = &textureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture


		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		std::string fileloc = basepath + filename;	/* Loading of image */
		success = ilLoadImage((const wchar_t*)fileloc.c_str());

		fprintf(stdout, "Loading Image: %s\n", fileloc.data());

		if (success) /* If no error occured: */
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
			unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
			if (!success)
			{
				/* Error occured */
				fprintf(stderr, "Couldn't convert image");
				return -1;
			}
			//glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
			glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
			//redefine standard texture values
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
			interpolation for magnification filter */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
			interpolation for minifying filter */
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
				ilGetData()); /* Texture specification */
		}
		else
		{
			/* Error occured */
			fprintf(stderr, "Couldn't load Image: %s\n", fileloc.data());
		}
	}
	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
	we can release memory used by image. */

	//Cleanup
	delete[] imageIds;
	imageIds = NULL;

	//return success;
	return TRUE;
}

int InitGL()					 // All Setup For OpenGL goes here
{
	if (!LoadGLTextures(scene))
	{
		return FALSE;
	}

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	return TRUE;					// Initialization Went OK
}

void keyboard(unsigned char key, int x, int y)
{
	float fraction = 0.1f;

	if (!menu) {					// Keys enabled in Room View
		switch (key) {
		case 27:
			exit(1);
			break;
		case 'a': // DO
			std::cout << "DO\n";
			piano->play2D("audios/do.wav", false);
			break;
		case 'w': // DO#
			std::cout << "DO#\n";
			piano->play2D("audios/do#.wav", false);
			break;
		case 's': // RE
			std::cout << "RE\n";
			piano->play2D("audios/re.wav", false);
			break;
		case 'e': // RE#
			std::cout << "RE#\n";
			piano->play2D("audios/re#.wav", false);
			break;
		case 'd': // MI
			std::cout << "MI\n";
			piano->play2D("audios/mi.wav", false);
			break;
		case 'f': // FA
			std::cout << "FA\n";
			piano->play2D("audios/fa.wav", false);
			break;
		case 't': // FA#
			std::cout << "FA#\n";
			piano->play2D("audios/fa#.wav", false);
			break;
		case 'g': // SOL
			std::cout << "SOL\n";
			piano->play2D("audios/sol.wav", false);
			break;
		case 'y': // SOL#
			std::cout << "SOL#\n";
			piano->play2D("audios/sol#.wav", false);
			break;
		case 'h': // LA
			std::cout << "LA\n";
			piano->play2D("audios/la.wav", false);
			break;
		case 'u': // LA#
			std::cout << "LA#\n";
			piano->play2D("audios/la#.wav", false);
			break;
		case 'j': // SI
			std::cout << "SI\n";
			piano->play2D("audios/si.wav", false);
			break;
		case 'k': // DO2
			std::cout << "DO2\n";
			piano->play2D("audios/do2.wav", false);
			break;
		case 'o': // DO#2
			std::cout << "DO#2\n";
			piano->play2D("audios/do#2.wav", false);
			break;
		case 'l': // RE2
			std::cout << "RE2\n";
			piano->play2D("audios/re2.wav", false);
			break;
		case 'p': // RE#2
			std::cout << "re#2\n";
			piano->play2D("audios/re#2.wav", false);
			break;

			// GESTIONE VISTA - Solo per testing

		case 'z': 
			height += 0.05;
			glutPostRedisplay();
			get_coords();
			break;
		case 'x':
			height -= 0.05;
			glutPostRedisplay();
			get_coords();
			break;
		default:
			break;
		}
	}

}

void movement(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		glutPostRedisplay();
		get_coords();
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		glutPostRedisplay();
		get_coords();
		break;
	case GLUT_KEY_UP:
		x += lx * fraction;
		z += lz * fraction;
		glutPostRedisplay();
		get_coords();
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fraction;
		z -= lz * fraction;
		glutPostRedisplay();
		get_coords();
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	//   x = (x - XRES / 2) * 2;
	//   y = (y - YRES / 2) * 2;

	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			if (x > 500 && x < 660 && y>(YRES - 30 - pos_start) && y < (YRES - pos_start)) {   //start new game
				menu = 0;
				glutPostRedisplay();
			}
			if (x > 495 && x < 670 && y>(YRES - 30 - pos_diff) && y < (YRES - pos_diff)) {   //set difficulty
				difficulty += 1;
				if (difficulty == 4) {
					difficulty = 1;
				}
				menu_display();
			}
		}
		break;
		/*   case GLUT_RIGHT_BUTTON:
			   if (state == GLUT_DOWN) {
				   y_o = 1.5;
				   if (x > 0 && y > 0)
					   y_o = -1.5;
				   glutPostRedisplay();
			   }
			   break;*/

	default:
		break;

	}
}

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	struct aiLogStream stream;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//	glutInitWindowPosition(100,100);
	glutInitWindowSize(XRES, YRES);
	glutCreateWindow("Assimp - Very simple OpenGL sample");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(movement);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);

	define_main_pitches();
	create_song();

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It will be active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);

	// ... exactly the same, but this stream will now write the
	// log file to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
	aiAttachLogStream(&stream);

	// the model name can be specified on the command line. 

	loadasset("../debug/models/room_2.obj");
	//loadasset("../debug/models/piano.obj");

	/*
	 if(argc>=2)
		loadasset( argv[1] );
	 else
	 {
		fprintf(stderr,"usage: >>SimpleOpenGLTexturedLoader <modelpath\\modelname>");
		exit(1);
	}

	*/

	if (!InitGL())
	{
		fprintf(stderr, "Initialization failed");
		return FALSE;
	}

	glutGet(GL_DEPTH_TEST);
	glutMainLoop();

	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(scene);

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
	return 0;
}
