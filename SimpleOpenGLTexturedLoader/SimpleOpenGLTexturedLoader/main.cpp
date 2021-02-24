#include "modelLoader.h"
#include "songs.h"
#include "controls.h"
#include <Windows.h>

void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;	// Font schermata menu
void* font2 = GLUT_BITMAP_HELVETICA_18;

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


void show_piano(void) {
	float tmp;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// angle = 28.31;

	//gluLookAt(x, height, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
	gluLookAt(-7.45f, 0, 7.5f, -7.45f, -2.f, 10, 0, 1, 0);

	// scale the whole asset to fit into our view frustum 
	 tmp = scene_max.x - scene_min.x;
	 tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	 tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	 tmp = 1.f / tmp;
	 //glScalef(tmp, tmp, tmp);

	glScalef(5,5,5);

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

	int diff = 2;
	if (difficulty == 1.0) {
		diff = 1;
	}
	if (difficulty == 2.0) {
		diff = 3;
	}

	char text_row_3[30] = "";
	std::string text_row_1 = "W E L C O M E";
	std::string text_row_2 = "Start new game";
	sprintf(text_row_3, "Select difficulty: %d", diff);
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
				difficulty = difficulty + 0.5;
				if (difficulty == 2.5) {
					difficulty = 1.0;
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





void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Display according to the flag:
	// 1 = Main Menù
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

	loadasset("../debug/models/stanza2.obj");
	// loadasset("../debug/models/room_2.obj");
	// loadasset("../debug/models/piano.obj");

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
