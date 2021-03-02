#include <stdlib.h>
#include "GL/glut.h"

extern int next_to_play;	//prossima nota che dovrebbe essere suonata
extern char nome[30];

void define_main_pitches();
void create_song(int song);
void play_song();
//float min(float a, float b);
int correct_pitch(unsigned char key);
void reset_all();

// SONGS
void twinkleTwinkle(void);
void fraMartino(void);
void ohSusanna(void);
void innoAllaGioia(void);

