
#ifndef HEADER
#define HEADER


Uint32 bouger_rond(Uint32 interval, void *param);
void jouer(void);
void initialiser_grille(void);

typedef struct matrice{
	int w, h, l;
	char **mat;
}matrice;


typedef struct obj{
	SDL_Rect pos[10];
	char dir, ind, indBonus[4][4], indPlanche, nbBonus[4], nbBallR, indBall[9], indEvent, indDef; //indices bonus, planche(grande-petite), nombre balles, événement(menu), et défoncement
	int lvl, continuer, score, tempsDef; //
	SDL_TimerID timer;
	SDL_Surface *ecran, *vert, *bleu, *blanc, *gris, *rouge, *plot, *ball[2]; //différentes surfaces blittées
	SDL_Surface *surface;  //surface de la balle
	double xspeed[10], yspeed[10]; // Vitesse de déplacement horizontal et vertical
	SDL_Rect posPlanche;
	double coeff[10];
	SDL_Surface *bonus[4];
	SDL_Rect posBonus[4][4], n[4][4];
}obj;

typedef struct bonus{
	SDL_Rect pos;
	char dir;
}bonus;

void blitter_surfaces(obj *rond, SDL_Surface *planche);
int create_matrice(char grille[16][16], obj *r);
int generer_bornes(int min, int max);

#endif