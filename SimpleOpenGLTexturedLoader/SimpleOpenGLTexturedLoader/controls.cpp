#include <iostream>
#include "controls.h"

double eyex = 0.f, eyey = 0.f, eyez = -3.f;
double centerx = 0.f, centery = 0.f, centerz = 5.f;
double upx = 0.f, upy = 1.f, upz = 0.f;
float angle_start = 28.3f;
float lx = 0.00433532, lz = 0.999991;
float x = -6.86088, z = 6.1202;
float angle = 28.27f;
float height = 3.f;
char menu = 1;
float difficulty = 1.5;							// 1 = EASY, 1.5 = MEDIUM, 2 = HARD
short int pos_titolo, pos_start, pos_diff;
ISoundEngine* piano = createIrrKlangDevice();

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

void get_coords(void) {
	std::cout << "gluLookAt(" << x << ", " << height << ", " << z << ", " << x + lx << ", centery, " << x + lz << ", " << upx << ", " << upy << ", " << upz << ");\n"
		<< "X " << x << " | Z " << z << " | LX " << lx << " | LZ " << lz << "| ANGLE " << angle << "\n";
}