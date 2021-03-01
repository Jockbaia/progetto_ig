#include <stdlib.h>
#include <irrKlang.h>
#include "GL/glut.h"

#define XRES 1280
#define YRES 720

//// LIBRERIA SONORA irrKlang
using namespace irrklang;
extern ISoundEngine* piano;
extern ISoundEngine* theme;

/// MENU COORDS
// Le variabili sono dichiarate in controls.cpp
extern double eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz;
extern float angle_start, lx, lz, x, z, angle, height;
extern char menu;
extern int difficulty;
extern short int pos_titolo, pos_start, pos_diff, pos_sel;
extern int punti, bonus, max_punti;
extern char message[10];

void keyboard(unsigned char key, int x, int y);
void movement(int key, int xx, int yy);
void get_coords(void);
void maintheme(int flag);