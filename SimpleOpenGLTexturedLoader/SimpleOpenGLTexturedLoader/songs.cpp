#include "songs.h"
#include "controls.h"
#include <iostream>

typedef struct n {
	char key;
	float x;
	struct n* next = NULL;
} nota_t;

typedef struct s {
	//std::string name = "Twinkle twinkle little star";
	float base_coord = 1;
	float offset = 0.25;
	float end;
	nota_t* note = NULL;
} song_t;

nota_t DO, RE, MI, FA, SOL, LA, SI, pause;
nota_t vett[100];
int n = 0, next_to_play = 0, consecutive = 0;
float speed = -0.0035;
song_t ttls;

int totalNotes;
float songPrecision;

void define_main_pitches() {
	//do
	DO.key = 'a';
	DO.x = -0.44;
	//re
	RE.key = 's';
	RE.x = -0.28;
	//mi
	MI.key = 'd';
	MI.x = -0.12;
	//fa
	FA.key = 'f';
	FA.x = 0;
	//sol
	SOL.key = 'g';
	SOL.x = 0.12;
	//la
	LA.key = 'h';
	LA.x = 0.28;
	//si
	SI.key = 'j';
	SI.x = 0.44;
	//pause (fake)
	pause.key = '_';
	pause.x = 3;

}

void create_song(int song) {
	if (song == 1) twinkleTwinkle();
	else if (song == 2) fraMartino();
}


void play_song() {
	//glClear(GL_COLOR_BUFFER_BIT);

	glScalef(1, 1, 1);
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
	glLoadIdentity();
	n = 0;

	while (n < totalNotes) {
		/*if (ttls.base_coord + ttls.offset * n > 2)
			break;*/
		glTranslatef(ttls.note[n].x, ttls.base_coord + n * ttls.offset, -2.5);
		glutSolidSphere(0.05, 4, 4);
		glTranslatef(-ttls.note[n].x, -ttls.base_coord - n * ttls.offset, 2.5);
		//glPopMatrix();
		n++;
	}
	switch (difficulty) {
	case 1:
		ttls.base_coord = ttls.base_coord + speed * songPrecision * 1.3;
		break;
	case 2:
		ttls.base_coord = ttls.base_coord + speed * songPrecision * 1.6;
		break;
	case 3:
		ttls.base_coord = ttls.base_coord + speed * songPrecision * 2.2;
		break;
	default:
		break;
	}

	if (ttls.base_coord <= ttls.end) {	//canzone finita
		menu = 1;
		sprintf_s(message, "");
		ttls.base_coord = 1;
		next_to_play = 0;
		consecutive = 0;
		bonus = 1;
		punti = 0;
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

int correct_pitch(unsigned char key) {
	//AGGIUNGI CONTROLLO SU POSIZIONE NOTA COMPRESA TRA LE LINEE 
	int res;
	float val;
	val = ttls.base_coord + ttls.offset * next_to_play;
	if (ttls.note[next_to_play].key == key) {
		if (val <= -0.1 && val >= -0.5) {	//Nota corretta premuta al momento giusto (dà bonus)
			consecutive++;
			if (consecutive == 5) {	//bonus accumulato dopo n note consecutive giuste
				consecutive = 0;
				bonus++;
			}
			next_to_play++;
			res = 1;
		}
		else if (val < -0.5) {	//Nota corretta suonata troppo tardi (azzera bonus ma non dà malus)
			consecutive = 0;
			bonus = 1;
			next_to_play++;
			res = 3;
		}
		else if (val > -0.1) {	//Nota corretta suonata troppo presto (azzera bonus ma non dà malus)
			consecutive = 0;
			bonus = 1;
			next_to_play++;
			res = 2;
		}

	}
	else {	//Nota sbagliata, passa a nota successiva (azzera bonus e dà malus)
		consecutive = 0;
		bonus = 1;
		next_to_play++;
		res = 0;
	}
	while (ttls.note[next_to_play].key == '_') {
		next_to_play++;
	}
	return res;
}

/*float min(float a, float b) {
	if (a <= b) {
		return a;
	}
	return b;
}*/


////////////////
// CANZONIERE //
////////////////

void twinkleTwinkle(void) {

	totalNotes = 48;
	songPrecision = 1;

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
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}

void fraMartino(void) {

	totalNotes = 64;
	songPrecision = 2.5;

	vett[0] = DO;
	vett[1] = pause;
	vett[2] = RE;
	vett[3] = pause;
	vett[4] = MI;
	vett[5] = pause;
	vett[6] = DO;
	vett[7] = pause;
	vett[8] = DO;
	vett[9] = pause;
	vett[10] = RE;
	vett[11] = pause;
	vett[12] = MI;
	vett[13] = pause;
	vett[14] = DO;
	vett[15] = pause;

	vett[16] = MI;
	vett[17] = pause;
	vett[18] = FA;
	vett[19] = pause;
	vett[20] = SOL;
	vett[21] = pause;
	vett[22] = pause;
	vett[23] = pause;
	vett[24] = MI;
	vett[25] = pause;
	vett[26] = FA;
	vett[27] = pause;
	vett[28] = SOL;
	vett[29] = pause;
	vett[30] = pause;
	vett[31] = pause;

	vett[32] = SOL;
	vett[33] = LA;
	vett[34] = SOL;
	vett[35] = FA;
	vett[36] = MI;
	vett[37] = pause;
	vett[38] = DO;
	vett[39] = pause;

	vett[40] = SOL;
	vett[41] = LA;
	vett[42] = SOL;
	vett[43] = FA;
	vett[44] = MI;
	vett[45] = pause;
	vett[46] = DO;
	vett[47] = pause;

	vett[48] = RE;
	vett[49] = pause;
	vett[50] = SOL;
	vett[51] = pause;
	vett[52] = DO;
	vett[53] = pause;
	vett[54] = pause;
	vett[55] = pause;

	vett[56] = RE;
	vett[57] = pause;
	vett[58] = SOL;
	vett[59] = pause;
	vett[60] = DO;
	vett[61] = pause;
	vett[62] = pause;
	vett[63] = pause;

	ttls.note = vett;
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}



