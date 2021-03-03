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
	float offset;
	float end;
	nota_t* note = NULL;
} song_t;

nota_t DO, RE, MI, FA, SOL, LA, SI, pause;
nota_t vett[500];
int n = 0, next_to_play = 0, consecutive = 0;
float speed = -0.0035;
song_t ttls;
char nome[30] = "";

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
	else if (song == 3) ohSusanna();
	else if (song == 4) innoAllaGioia();
}


void play_song() {
	//glClear(GL_COLOR_BUFFER_BIT);

	glScalef(1, 1, 1);
	glColor3f(0.0, 1.0, 1.0);
	glPushMatrix();
	glLoadIdentity();
	n = next_to_play;

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
		if (max_punti < punti)
			max_punti = punti; 
		reset_all();
		piano->play2D("audios/applause.wav", false);
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
	int res;
	float val;

	if (ttls.note[next_to_play].key == '_') { //per ohSusanna che inizia con pausa
		next_to_play++;
	}
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
	else {	//Nota sbagliata, aspetta nota giusta (azzera bonus e dà malus)
		consecutive = 0;
		bonus = 1;
		//next_to_play++;
		res = 0;
	}
	while (ttls.note[next_to_play].key == '_') {
		next_to_play++;
	}
	return res;
}

void reset_all() {
	menu = 1;
	sprintf_s(message, "");
	ttls.base_coord = 1;
	next_to_play = 0;
	consecutive = 0;
	bonus = 1;
	punti = 0;
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
	songPrecision = 1.1;
	sprintf_s(nome, "Twinkle Twinkle Little Star");

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
	ttls.offset = 0.25;
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}

void fraMartino(void) {

	totalNotes = 64;
	songPrecision = 1.5;
	sprintf(nome, "Fra Martino");

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
	ttls.offset = 0.2;
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}

void ohSusanna(void) {

	totalNotes = 381;
	//songPrecision = 18;
	songPrecision = 1.8;
	sprintf_s(nome, "Oh Susanna");

	vett[0] = pause;
	vett[1] = DO;
	vett[2] = pause;
	vett[3] = RE;
	vett[4] = pause;
	vett[5] = MI;
	vett[6] = pause;
	vett[7] = pause;
	vett[8] = pause;
	vett[9] = SOL;
	vett[10] = pause;
	vett[11] = pause;
	vett[12] = pause;
	vett[13] = SOL;
	vett[14] = pause;
	vett[15] = pause;
	vett[16] = pause;
	vett[17] = LA;
	vett[18] = pause;
	vett[19] = pause;
	vett[20] = pause;
	vett[21] = SOL;
	vett[22] = pause;
	vett[23] = pause;
	vett[24] = pause;
	vett[25] = MI;
	vett[26] = pause;
	vett[27] = pause;
	vett[28] = pause;
	vett[29] = DO;
	vett[30] = pause;
	vett[31] = pause;
	vett[32] = pause;
	vett[33] = pause;
	vett[34] = pause;
	vett[35] = RE;
	vett[36] = pause;
	vett[37] = MI;
	vett[38] = pause;
	vett[39] = pause;
	vett[40] = pause;
	vett[41] = MI;
	vett[42] = pause;
	vett[43] = pause;
	vett[44] = pause;
	vett[45] = RE;
	vett[46] = pause;
	vett[47] = pause;
	vett[48] = pause;
	vett[49] = DO;
	vett[50] = pause;
	vett[51] = pause;
	vett[52] = pause;
	vett[53] = RE;
	vett[54] = pause;
	vett[55] = pause;
	vett[56] = pause;
	vett[57] = pause;
	vett[58] = pause;
	vett[59] = pause;
	vett[60] = pause;
	vett[61] = pause;
	vett[62] = pause;
	vett[63] = pause;
	vett[64] = pause;
	vett[65] = DO;
	vett[66] = pause;
	vett[67] = RE;
	vett[68] = pause;
	vett[69] = MI;
	vett[70] = pause;
	vett[71] = pause;
	vett[72] = pause;
	vett[73] = SOL;
	vett[74] = pause;
	vett[75] = pause;
	vett[76] = pause;
	vett[77] = SOL;
	vett[78] = pause;
	vett[79] = pause;
	vett[80] = pause;
	vett[81] = LA;
	vett[82] = pause;
	vett[83] = pause;
	vett[84] = pause;
	vett[85] = SOL;
	vett[86] = pause;
	vett[87] = pause;
	vett[88] = pause;
	vett[89] = MI;
	vett[90] = pause;
	vett[91] = pause;
	vett[92] = pause;
	vett[93] = DO;
	vett[94] = pause;
	vett[95] = pause;
	vett[96] = pause;
	vett[97] = pause;
	vett[98] = pause;
	vett[99] = RE;
	vett[100] = pause;
	vett[101] = MI;
	vett[102] = pause;
	vett[103] = pause;
	vett[104] = pause;
	vett[105] = MI;
	vett[106] = pause;
	vett[107] = pause;
	vett[108] = pause;
	vett[109] = RE;
	vett[110] = pause;
	vett[111] = pause;
	vett[112] = pause;
	vett[113] = RE;
	vett[114] = pause;
	vett[115] = pause;
	vett[116] = pause;
	vett[117] = DO;
	vett[118] = pause;
	vett[119] = pause;
	vett[120] = pause;
	vett[121] = pause;
	vett[122] = pause;
	vett[123] = pause;
	vett[124] = pause;
	vett[125] = pause;
	vett[126] = pause;
	vett[127] = pause;
	vett[128] = pause;

	vett[129] = DO;
	vett[130] = pause;
	vett[131] = RE;
	vett[132] = pause;
	vett[133] = MI;
	vett[134] = pause;
	vett[135] = pause;
	vett[136] = pause;
	vett[137] = SOL;
	vett[138] = pause;
	vett[139] = pause;
	vett[140] = pause;
	vett[141] = SOL;
	vett[142] = pause;
	vett[143] = pause;
	vett[144] = pause;
	vett[145] = LA;
	vett[146] = pause;
	vett[147] = pause;
	vett[148] = pause;
	vett[149] = SOL;
	vett[150] = pause;
	vett[151] = pause;
	vett[152] = pause;
	vett[153] = MI;
	vett[154] = pause;
	vett[155] = pause;
	vett[156] = pause;
	vett[157] = DO;
	vett[158] = pause;
	vett[159] = pause;
	vett[160] = pause;
	vett[161] = pause;
	vett[162] = pause;
	vett[163] = RE;
	vett[164] = pause;
	vett[165] = MI;
	vett[166] = pause;
	vett[167] = pause;
	vett[168] = pause;
	vett[169] = MI;
	vett[170] = pause;
	vett[171] = pause;
	vett[172] = pause;
	vett[173] = RE;
	vett[174] = pause;
	vett[175] = pause;
	vett[176] = pause;
	vett[177] = DO;
	vett[178] = pause;
	vett[179] = pause;
	vett[180] = pause;
	vett[181] = RE;
	vett[182] = pause;
	vett[183] = pause;
	vett[184] = pause;
	vett[185] = pause;
	vett[186] = pause;
	vett[187] = pause;
	vett[188] = pause;
	vett[189] = pause;
	vett[190] = pause;
	vett[191] = pause;
	vett[192] = pause;
	vett[193] = DO;
	vett[194] = pause;
	vett[195] = RE;
	vett[196] = pause;
	vett[197] = MI;
	vett[198] = pause;
	vett[199] = pause;
	vett[200] = pause;
	vett[201] = SOL;
	vett[202] = pause;
	vett[203] = pause;
	vett[204] = pause;
	vett[205] = SOL;
	vett[206] = pause;
	vett[207] = pause;
	vett[208] = pause;
	vett[209] = LA;
	vett[210] = pause;
	vett[211] = pause;
	vett[212] = pause;
	vett[213] = SOL;
	vett[214] = pause;
	vett[215] = pause;
	vett[216] = pause;
	vett[217] = MI;
	vett[218] = pause;
	vett[219] = pause;
	vett[220] = pause;
	vett[221] = DO;
	vett[222] = pause;
	vett[223] = pause;
	vett[224] = pause;
	vett[225] = pause;
	vett[226] = pause;
	vett[227] = RE;
	vett[228] = pause;
	vett[229] = MI;
	vett[230] = pause;
	vett[231] = pause;
	vett[232] = pause;
	vett[233] = MI;
	vett[234] = pause;
	vett[235] = pause;
	vett[236] = pause;
	vett[237] = RE;
	vett[238] = pause;
	vett[239] = pause;
	vett[240] = pause;
	vett[241] = RE;
	vett[242] = pause;
	vett[243] = pause;
	vett[244] = pause;
	vett[245] = DO;
	vett[246] = pause;
	vett[247] = pause;
	vett[248] = pause;
	vett[249] = pause;
	vett[250] = pause;
	vett[251] = pause;
	vett[252] = pause;
	vett[253] = pause;
	vett[254] = pause;
	vett[255] = pause;
	vett[256] = pause;

	vett[257] = FA;
	vett[258] = pause;
	vett[259] = pause;
	vett[260] = pause;
	vett[261] = pause;
	vett[262] = pause;
	vett[263] = pause;
	vett[264] = pause;
	vett[265] = FA;
	vett[266] = pause;
	vett[267] = pause;
	vett[268] = pause;
	vett[269] = pause;
	vett[270] = pause;
	vett[271] = pause;
	vett[272] = pause;
	vett[273] = LA;
	vett[274] = pause;
	vett[275] = pause;
	vett[276] = pause;
	vett[277] = LA;
	vett[278] = pause;
	vett[279] = pause;
	vett[280] = pause;
	vett[281] = pause;
	vett[282] = pause;
	vett[283] = pause;
	vett[284] = pause;
	vett[285] = LA;
	vett[286] = pause;
	vett[287] = pause;
	vett[288] = pause;
	vett[289] = SOL;
	vett[290] = pause;
	vett[291] = pause;
	vett[292] = pause;
	vett[293] = SOL;
	vett[294] = pause;
	vett[295] = pause;
	vett[296] = pause;
	vett[297] = MI;
	vett[298] = pause;
	vett[299] = pause;
	vett[300] = pause;
	vett[301] = DO;
	vett[302] = pause;
	vett[303] = pause;
	vett[304] = pause;
	vett[305] = RE;
	vett[306] = pause;
	vett[307] = pause;
	vett[308] = pause;
	vett[309] = pause;
	vett[310] = pause;
	vett[311] = pause;
	vett[312] = pause;
	vett[313] = pause;
	vett[314] = pause;
	vett[315] = pause;
	vett[316] = pause;

	vett[317] = DO;
	vett[318] = pause;
	vett[319] = RE;
	vett[320] = pause;
	vett[321] = MI;
	vett[322] = pause;
	vett[323] = pause;
	vett[324] = pause;
	vett[325] = SOL;
	vett[326] = pause;
	vett[327] = pause;
	vett[328] = pause;
	vett[329] = SOL;
	vett[330] = pause;
	vett[331] = pause;
	vett[332] = pause;
	vett[333] = LA;
	vett[334] = pause;
	vett[335] = pause;
	vett[336] = pause;
	vett[337] = SOL;
	vett[338] = pause;
	vett[339] = pause;
	vett[340] = pause;
	vett[341] = MI;
	vett[342] = pause;
	vett[343] = pause;
	vett[344] = pause;
	vett[345] = DO;
	vett[346] = pause;
	vett[347] = pause;
	vett[348] = pause;
	vett[349] = pause;
	vett[350] = pause;
	vett[351] = RE;
	vett[352] = pause;
	vett[353] = MI;
	vett[354] = pause;
	vett[355] = pause;
	vett[356] = pause;
	vett[357] = MI;
	vett[358] = pause;
	vett[359] = pause;
	vett[360] = pause;
	vett[361] = RE;
	vett[362] = pause;
	vett[363] = pause;
	vett[364] = pause;
	vett[365] = RE;
	vett[366] = pause;
	vett[367] = pause;
	vett[368] = pause;
	vett[369] = DO;
	vett[370] = pause;
	vett[371] = pause;
	vett[372] = pause;
	vett[373] = pause;
	vett[374] = pause;
	vett[375] = pause;
	vett[376] = pause;
	vett[377] = pause;
	vett[378] = pause;
	vett[379] = pause;
	vett[380] = pause;

	ttls.note = vett;
	ttls.offset = 0.1;
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}

void innoAllaGioia(void) {

	totalNotes = 256;
	//songPrecision = 18;
	songPrecision = 1.8;
	sprintf_s(nome, "Inno alla Gioia");

	vett[0] = MI;
	vett[1] = pause;
	vett[2] = pause;
	vett[3] = pause;
	vett[4] = MI;
	vett[5] = pause;
	vett[6] = pause;
	vett[7] = pause;
	vett[8] = FA;
	vett[9] = pause;
	vett[10] = pause;
	vett[11] = pause;
	vett[12] = SOL;
	vett[13] = pause;
	vett[14] = pause;
	vett[15] = pause;
	vett[16] = SOL;
	vett[17] = pause;
	vett[18] = pause;
	vett[19] = pause;
	vett[20] = FA;
	vett[21] = pause;
	vett[22] = pause;
	vett[23] = pause;
	vett[24] = MI;
	vett[25] = pause;
	vett[26] = pause;
	vett[27] = pause;
	vett[28] = RE;
	vett[29] = pause;
	vett[30] = pause;
	vett[31] = pause;
	vett[32] = DO;
	vett[33] = pause;
	vett[34] = pause;
	vett[35] = pause;
	vett[36] = DO;
	vett[37] = pause;
	vett[38] = pause;
	vett[39] = pause;
	vett[40] = RE;
	vett[41] = pause;
	vett[42] = pause;
	vett[43] = pause;
	vett[44] = MI;
	vett[45] = pause;
	vett[46] = pause;
	vett[47] = pause;
	vett[48] = MI;
	vett[49] = pause;
	vett[50] = pause;
	vett[51] = pause;
	vett[52] = pause;
	vett[53] = pause;
	vett[54] = RE;
	vett[55] = pause;
	vett[56] = RE;
	vett[57] = pause;
	vett[58] = pause;
	vett[59] = pause;
	vett[60] = pause;
	vett[61] = pause;
	vett[62] = pause;
	vett[63] = pause;
	vett[64] = MI;
	vett[65] = pause;
	vett[66] = pause;
	vett[67] = pause;
	vett[68] = MI;
	vett[69] = pause;
	vett[70] = pause;
	vett[71] = pause;
	vett[72] = FA;
	vett[73] = pause;
	vett[74] = pause;
	vett[75] = pause;
	vett[76] = SOL;
	vett[77] = pause;
	vett[78] = pause;
	vett[79] = pause;
	vett[80] = SOL;
	vett[81] = pause;
	vett[82] = pause;
	vett[83] = pause;
	vett[84] = FA;
	vett[85] = pause;
	vett[86] = pause;
	vett[87] = pause;
	vett[88] = MI;
	vett[89] = pause;
	vett[90] = pause;
	vett[91] = pause;
	vett[92] = RE;
	vett[93] = pause;
	vett[94] = pause;
	vett[95] = pause;
	vett[96] = DO;
	vett[97] = pause;
	vett[98] = pause;
	vett[99] = pause;
	vett[100] = DO;
	vett[101] = pause;
	vett[102] = pause;
	vett[103] = pause;
	vett[104] = RE;
	vett[105] = pause;
	vett[106] = pause;
	vett[107] = pause;
	vett[108] = MI;
	vett[109] = pause;
	vett[110] = pause;
	vett[111] = pause;
	vett[112] = RE;
	vett[113] = pause;
	vett[114] = pause;
	vett[115] = pause;
	vett[116] = pause;
	vett[117] = pause;
	vett[118] = DO;
	vett[119] = pause;
	vett[120] = DO;
	vett[121] = pause;
	vett[122] = pause;
	vett[123] = pause;
	vett[124] = pause;
	vett[125] = pause;
	vett[126] = pause;
	vett[127] = pause;

	vett[128] = RE;
	vett[129] = pause;
	vett[130] = pause;
	vett[131] = pause;
	vett[132] = RE;
	vett[133] = pause;
	vett[134] = pause;
	vett[135] = pause;
	vett[136] = MI;
	vett[137] = pause;
	vett[138] = pause;
	vett[139] = pause;
	vett[140] = DO;
	vett[141] = pause;
	vett[142] = pause;
	vett[143] = pause;
	vett[144] = RE;
	vett[145] = pause;
	vett[146] = pause;
	vett[147] = pause;
	vett[148] = MI;
	vett[149] = pause;
	vett[150] = FA;
	vett[151] = pause;
	vett[152] = MI;
	vett[153] = pause;
	vett[154] = pause;
	vett[155] = pause;
	vett[156] = DO;
	vett[157] = pause;
	vett[158] = pause;
	vett[159] = pause;
	vett[160] = RE;
	vett[161] = pause;
	vett[162] = pause;
	vett[163] = pause;
	vett[164] = MI;
	vett[165] = pause;
	vett[166] = FA;
	vett[167] = pause;
	vett[168] = MI;
	vett[169] = pause;
	vett[170] = pause;
	vett[171] = pause;
	vett[172] = RE;
	vett[173] = pause;
	vett[174] = pause;
	vett[175] = pause;
	vett[176] = DO;
	vett[177] = pause;
	vett[178] = pause;
	vett[179] = pause;
	vett[180] = RE;
	vett[181] = pause;
	vett[182] = pause;
	vett[183] = pause;
	vett[184] = RE;
	vett[185] = pause;
	vett[186] = pause;
	vett[187] = pause;
	vett[188] = pause;
	vett[189] = pause;
	vett[190] = pause;
	vett[191] = pause;
	vett[192] = MI;
	vett[193] = pause;
	vett[194] = pause;
	vett[195] = pause;
	vett[196] = MI;
	vett[197] = pause;
	vett[198] = pause;
	vett[199] = pause;
	vett[200] = FA;
	vett[201] = pause;
	vett[202] = pause;
	vett[203] = pause;
	vett[204] = SOL;
	vett[205] = pause;
	vett[206] = pause;
	vett[207] = pause;
	vett[208] = SOL;
	vett[209] = pause;
	vett[210] = pause;
	vett[211] = pause;
	vett[212] = FA;
	vett[213] = pause;
	vett[214] = pause;
	vett[215] = pause;
	vett[216] = MI;
	vett[217] = pause;
	vett[218] = pause;
	vett[219] = pause;
	vett[220] = RE;
	vett[221] = pause;
	vett[222] = pause;
	vett[223] = pause;
	vett[224] = DO;
	vett[225] = pause;
	vett[226] = pause;
	vett[227] = pause;
	vett[228] = DO;
	vett[229] = pause;
	vett[230] = pause;
	vett[231] = pause;
	vett[232] = RE;
	vett[233] = pause;
	vett[234] = pause;
	vett[235] = pause;
	vett[236] = MI;
	vett[237] = pause;
	vett[238] = pause;
	vett[239] = pause;
	vett[240] = RE;
	vett[241] = pause;
	vett[242] = pause;
	vett[243] = pause;
	vett[244] = pause;
	vett[245] = pause;
	vett[246] = DO;
	vett[247] = pause;
	vett[248] = DO;
	vett[249] = pause;
	vett[250] = pause;
	vett[251] = pause;
	vett[252] = pause;
	vett[253] = pause;
	vett[254] = pause;
	vett[255] = pause;

	ttls.note = vett;
	ttls.offset = 0.1;
	ttls.end = -1 * (ttls.base_coord + totalNotes * ttls.offset);
}



