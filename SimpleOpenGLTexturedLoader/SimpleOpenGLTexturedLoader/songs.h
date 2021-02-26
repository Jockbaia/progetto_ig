#include <stdlib.h>
#include "GL/glut.h"

extern int next_to_play;	//prossima nota che dovrebbe essere suonata

void define_main_pitches();
void create_song(int song);
void play_song();
//float min(float a, float b);
int correct_pitch(unsigned char key);

// SONGS
void twinkleTwinkle(void);
void fraMartino(void);

