#include "modelLoader.h"
#include "songs.h"
#include "controls.h"
#include <Windows.h>
#include <iostream>

void* font1 = GLUT_BITMAP_TIMES_ROMAN_24;	// Font schermata menu
void* font2 = GLUT_BITMAP_HELVETICA_18;
int song = 3;

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

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// angle = 28.31;

	// gluLookAt(x, height, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
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
	glTranslatef(0,-0.5,-2.5);
	glutSolidSphere(0.02, 8, 4);
	glPopMatrix();*/

	play_song();
//	glutSwapBuffers();
	//glutPostRedisplay();
}

void show_ipad(void) {
	float tmp;

	x = -5.12236, z = 11.2895, lx = 0.261272, lz = 0.965265;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, 4.25, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);

	// scale the whole asset to fit into our view frustum 
	tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	tmp = 1.f / tmp;

	glScalef(5, 5, 5);

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

}

void show_instructions(void) {
	float tmp;
	x = -8.65f, z = 7.f, lx = -0.233497, lz = 0.972358;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, -0.7f, z, x + lx, -1.f, z + lz, 0.0f, 1.0f, 0.0f);
	
	

	// scale the whole asset to fit into our view frustum 
	tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	tmp = 1.f / tmp;

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

/*	int diff = 2;
	if (difficulty == 1.0) {
		diff = 1;
	}
	if (difficulty == 2.0) {
		diff = 3;
	}*/

	char text_row_3[30] = "";
	char text_row_1[20] = ""; // "W E L C O M E";
	std::string text_row_2 = "Start new game";
	sprintf_s(text_row_3, "Select difficulty: %d", difficulty);
	std::string text_row_4 = "How to Play";
	
	if (menu == 1) {
		pos_titolo = 120;
		//output(500, pos_titolo, text_row_1, font1);
		pos_start = 220;
		output(500, pos_start, text_row_2, font2);
		pos_sel = 160;
		output(510, pos_sel, "Select song", font2);
		pos_diff = 100;
		output(495, pos_diff, text_row_3, font2);
		output(515, pos_diff - 55, text_row_4, font2);
		output(20, pos_titolo, "Current song: ", font2);
		output(150, pos_titolo, nome, font2);
		sprintf_s(text_row_1, "High score: %d", max_punti);
		output(1100, pos_titolo, text_row_1, font2);
	}
	else if (menu == 2) {
		pos_titolo = 525;
		pos_start = 460;
		pos_sel = 390;
		pos_diff = 315;
//		output(670, pos_titolo,	"                          ", font2);
//		output(670, pos_start,	"                          ", font2);
//		output(670, pos_sel,	"                          ", font2);
//		output(670, pos_diff,	"                          ", font2);
	}
	

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	if (menu == 1) show_room();
	else if (menu == 2) show_ipad();
	else if (menu == 3) show_instructions();

	glutSwapBuffers();
}

void game_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(1.0, XRES, 1.0, YRES);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	char points[12] = "";
	sprintf_s(points, "Score: %d", punti);
	output(20, 650, points, font1);
	if (bonus != 1) {
		sprintf_s(points, "Bonus: %d", bonus);
		output(20, 600, points, font1);
	}

	output(XRES / 2 - 10, YRES / 2 + 50, message, font2);
	output(XRES - 265, YRES - 265, "Back to menu", font2);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glColor3f(1, 0, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(-2, -0.15, -2.6);
	glVertex3f(2, -0.15, -2.6);
	glVertex3f(2, -0.5, -2.6);
	glVertex3f(-2, -0.5, -2.6);
	glEnd();

	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex3f(1.1, 0.25, -2.6);
	glVertex3f(1.48, 0.25, -2.6);
	glVertex3f(1.48, 0.35, -2.6);
	glVertex3f(1.1, 0.35, -2.6);
	glEnd();

	show_piano();
	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	//   x = (x - XRES / 2) * 2;
	//   y = (y - YRES / 2) * 2;

	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && menu == 1) {
			if (x > 500 && x < 660 && y>(YRES - 30 - pos_start) && y < (YRES - pos_start)) {   //start new game
				std::cout << "CLICK! Inizia a giocare\n";
				piano->play2D("audios/click.wav", false);
				menu = 0;
				glutPostRedisplay();
			}
			if (x > 495 && x < 670 && y>(YRES - 30 - pos_diff) && y < (YRES - pos_diff)) {   //set difficulty
				std::cout << "CLICK! Cambio Difficoltà\n";
				piano->play2D("audios/click.wav", false);
				difficulty = difficulty + 1;
				if (difficulty == 4) {
					difficulty = 1;
				}
				menu_display();
			}
			if (x > 510 && x<650 && y>(YRES - 30 - pos_sel) && y < (YRES - pos_sel)) {	//show song menu
				std::cout << "CLICK! Menù Canzoni\n";
				piano->play2D("audios/click.wav", false);
				menu = 2;
				glutPostRedisplay();
			}

			if (x > 515 && x<650 && y>(YRES - pos_diff + 45) && y < (YRES - pos_diff + 100)) {	//show instructions
				std::cout << "CLICK! Istruzioni di Gioco\n";
				piano->play2D("audios/paper1.wav", false);
				menu = 3;
				glutPostRedisplay();
			}
		}
		else if (state == GLUT_DOWN && menu == 2) {

			if (x > 670 && x<870 && y>(YRES - 30 - pos_titolo) && y < (YRES - pos_titolo)) {
				std::cout << "SONG 1 selected\n";
				piano->play2D("audios/tap.wav", false);
				song = 1;
				create_song(song);

			}
			if (x > 670 && x<870 && y>(YRES - 30 - pos_start) && y < (YRES - pos_start)) {
				std::cout << "SONG 2 selected\n";
				piano->play2D("audios/tap.wav", false);
				song = 2;
				create_song(song);

			}
			if (x > 670 && x<870 && y>(YRES - 30 - pos_sel) && y < (YRES - pos_sel)) {
				std::cout << "SONG 3 selected\n";
				piano->play2D("audios/tap.wav", false);
				song = 3;
				create_song(song);

			}
			if (x > 670 && x<870 && y>(YRES - 30 - pos_diff) && y < (YRES - pos_diff)) {
				std::cout << "SONG 4 selected\n";
				piano->play2D("audios/tap.wav", false);
				song = 4;
				create_song(song);

			}
			menu = 1;
			glutPostRedisplay();
		}
		else if (state == GLUT_DOWN && menu == 3) {

			if (x > 0 && x<XRES && y>0 && y < YRES) {
				std::cout << "Ritorno al menù principale\n";
				piano->play2D("audios/paper2.wav", false);
				menu = 1;
			}
			
			
			glutPostRedisplay();
		}
		else if (state == GLUT_DOWN && menu == 0) {
			if (x > XRES - 265 && x < XRES - 140  && y < 275 && y > 245) {
				menu = 1;
				reset_all();
				glutPostRedisplay();
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
	// 2 = Song Selection
	// 0 = Free-roaming / game
	
	

	if (menu == 1) {
		maintheme(1);
		menu_display();
		show_room();
	}
	else if (menu == 2) {
		maintheme(1);
		menu_display();
		show_ipad();
	}
	else if (menu == 3) {
		maintheme(1);
		menu_display();
		show_instructions();
	}
	else {
		maintheme(0);
		game_display();
		show_piano();
	}
}

int main(int argc, char** argv)
{
	struct aiLogStream stream;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//	glutInitWindowPosition(100,100);
	glutInitWindowSize(XRES, YRES);
	glutCreateWindow("myPiano Training - Informatica Grafica");

	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(movement);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);

	define_main_pitches();
	create_song(song);

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

	loadasset("../debug/models/stanzafinal.obj");
	// loadasset("../debug/models/stanza46.obj");
	// loadasset("../debug/models/stanza2.obj");
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
