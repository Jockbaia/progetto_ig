#include "songs.h"
#include "controls.h"

typedef struct n {
	char key;
	float x;
	struct n* next = NULL;
} nota_t;

typedef struct s {
	//std::string name = "Twinkle twinkle little star";
	float base_coord = 1;
	float offset = 0.25;
	nota_t* note = NULL;
} song_t;

nota_t DO, RE, MI, FA, SOL, LA, SI, pause;
nota_t vett[50];
int n = 0;
float speed = -0.0035;
song_t ttls;


void define_main_pitches() {
	//do
	DO.key = 'A';
	DO.x = -0.44;
	//re
	RE.key = 'S';
	RE.x = -0.28;
	//mi
	MI.key = 'D';
	MI.x = -0.12;
	//fa
	FA.key = 'F';
	FA.x = 0;
	//sol
	SOL.key = 'G';
	SOL.x = 0.12;
	//la
	LA.key = 'H';
	LA.x = 0.28;
	//si
	SI.key = 'J';
	SI.x = 0.44;
	//pause (fake)
	pause.key = '_';
	pause.x = 3;

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

float min(float a, float b) {
	if (a <= b) {
		return a;
	}
	return b;
}


void play_song(/*song_t canzone*/) {
	//glClear(GL_COLOR_BUFFER_BIT);

	glScalef(1, 1, 1);
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
	glLoadIdentity();
	n = 0;
	while (n < 48) {
		/*if (ttls.base_coord + ttls.offset * n > 2)
			break;*/
		glTranslatef(ttls.note[n].x, ttls.base_coord + n * ttls.offset, -2.5);
		glutSolidSphere(0.05, 4, 4);
		glTranslatef(-ttls.note[n].x, -ttls.base_coord - n * ttls.offset, 2.5);
		//glPopMatrix();
		n++;
	}
	ttls.base_coord = ttls.base_coord + speed * difficulty;
	if (ttls.base_coord <= -12.8) {
		menu = 1;
		ttls.base_coord = 1;
	}
	/*while (n < 48) {
		glTranslatef(ttls.note[n].x, -1 + n * ttls.offset, -3);
		glutSolidSphere(0.03, 4, 4);
		glTranslatef(-ttls.note[n].x, +1 - n * ttls.offset, 3);
		glPopMatrix();
		n++;
	}*/
	glutSwapBuffers();
	glutPostRedisplay();
}