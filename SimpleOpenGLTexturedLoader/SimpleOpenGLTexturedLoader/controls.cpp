#include <iostream>
#include "controls.h"
#include "songs.h"

double eyex = 0.f, eyey = 0.f, eyez = -3.f;
double centerx = 0.f, centery = 0.f, centerz = 5.f;
double upx = 0.f, upy = 1.f, upz = 0.f;
float angle_start = 28.3f;
float lx = 0.00433532, lz = 0.999991;
float x = -6.86088, z = 6.1202;
float angle = 28.27f;
float height = 3.f;
char menu = 1;
int difficulty = 2;							// 1 = EASY, 2 = MEDIUM, 3 = HARD
int punti = 0, bonus = 1, max_punti = 0;
short int pos_titolo, pos_start, pos_diff, pos_sel;
char message[10] = "";
ISoundEngine* piano = createIrrKlangDevice();
ISoundEngine* theme = createIrrKlangDevice();
int isPlaying = 0;

void output(int x, int y, std::string str)
{
	int len, i;

	glRasterPos2f(x, y);        //set text position in the window
	len = str.length();

	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	//glutSwapBuffers();
	//glutPostRedisplay();
}


void calcola_punti(unsigned char key) {
	int res = correct_pitch(key);
	switch (res) {
	case 0:
		//output(XRES / 2 - 10, YRES / 2 + 50, "BAD!");
		sprintf_s(message, "BAD!");
		punti--;
		break;
	case 1:
		//output(XRES / 2 - 20, YRES / 2 + 50, "PERFECT!");
		sprintf_s(message, "PERFECT!");
		punti = punti + bonus;	//normalmente +1, se c'è bonus +n
		break;
	case 2:
		//output(XRES / 2 - 20, YRES / 2 + 50, "Too soon");
		sprintf_s(message, "Too soon");
		punti = punti + bonus;	//normalmente +1, se c'è bonus +n
		break;
	case 3:
		//output(XRES / 2 - 20, YRES / 2 + 50, "Too late");
		sprintf_s(message, "Too late");
		punti = punti + bonus;	//normalmente +1, se c'è bonus +n
		break;
	default:
		break;
	}
/*	if (correct_pitch(key)) {
		punti = punti + bonus;	//normalmente +1, se c'è bonus +n
	}
	else
	{
		punti--;
	}*/
	return;
}

void keyboard(unsigned char key, int x, int y)
{
	float fraction = 0.1f;

	if (!menu) { // Keys enabled in Room View
		switch (key) {
		case 27:
			exit(1);
			break;
		case 'a': // DO
			std::cout << "DO\n";
			piano->play2D("audios/do.wav", false);
			calcola_punti(key);
			break;
		case 'w': // DO#
			std::cout << "DO#\n";
			piano->play2D("audios/do#.wav", false);
			calcola_punti(key);
			break;
		case 's': // RE
			std::cout << "RE\n";
			piano->play2D("audios/re.wav", false);
			calcola_punti(key);
			break;
		case 'e': // RE#
			std::cout << "RE#\n";
			piano->play2D("audios/re#.wav", false);
			calcola_punti(key);
			break;
		case 'd': // MI
			std::cout << "MI\n";
			piano->play2D("audios/mi.wav", false);
			calcola_punti(key);
			break;
		case 'f': // FA
			std::cout << "FA\n";
			piano->play2D("audios/fa.wav", false);
			calcola_punti(key);
			break;
		case 't': // FA#
			std::cout << "FA#\n";
			piano->play2D("audios/fa#.wav", false);
			calcola_punti(key);
			break;
		case 'g': // SOL
			std::cout << "SOL\n";
			piano->play2D("audios/sol.wav", false);
			calcola_punti(key);
			break;
		case 'y': // SOL#
			std::cout << "SOL#\n";
			piano->play2D("audios/sol#.wav", false);
			calcola_punti(key);
			break;
		case 'h': // LA
			std::cout << "LA\n";
			piano->play2D("audios/la.wav", false);
			calcola_punti(key);
			break;
		case 'u': // LA#
			std::cout << "LA#\n";
			piano->play2D("audios/la#.wav", false);
			calcola_punti(key);
			break;
		case 'j': // SI
			std::cout << "SI\n";
			piano->play2D("audios/si.wav", false);
			calcola_punti(key);
			break;
		case 'k': // DO2
			std::cout << "DO2\n";
			piano->play2D("audios/do2.wav", false);
			calcola_punti(key);
			break;
		case 'o': // DO#2
			std::cout << "DO#2\n";
			piano->play2D("audios/do#2.wav", false);
			calcola_punti(key);
			break;
		case 'l': // RE2
			std::cout << "RE2\n";
			piano->play2D("audios/re2.wav", false);
			calcola_punti(key);
			break;
		case 'p': // RE#2
			std::cout << "re#2\n";
			piano->play2D("audios/re#2.wav", false);
			calcola_punti(key);
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
	else if (key == 27) {//per poter uscire anche dal menu
		exit(1);
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

void get_coords(void) {
	std::cout << "gluLookAt(" << x << ", " << height << ", " << z << ", " << x + lx << ", centery, " << x + lz << ", " << upx << ", " << upy << ", " << upz << ");\n"
		<< "X " << x << " | Z " << z << " | LX " << lx << " | LZ " << lz << "| ANGLE " << angle << "\n";
}

void maintheme(int flag) {
	if (flag) {
		if (isPlaying == 0) {
			theme->play2D("audios/main.wav", false);
		}
		isPlaying++;
	}
	if (!flag && isPlaying) {
		theme->stopAllSounds();
		isPlaying = 0;
	}
	
}