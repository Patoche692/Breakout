
#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Header.h"


int create_matrice(char grille[16][16], obj *r)
{
	int abs = 0, ord = 0, absu = 0, ordu = 0, absd = 0, ordd = 0;
	int n = 0, p = 0, m = 0, k = 0;
	FILE* read = NULL;

	switch (r->lvl)
	{
	case 0:
		read = fopen("level1.txt", "r");
		break;
	case 1:
		read = fopen("level2.txt", "r");
		break;
	case 2:
		read = fopen("level3.txt", "r");
		break;
	case 3:
		read = fopen("level4.txt", "r");
		break;
	}
	if (!read)
		return NULL;

	char getc = 0;

	for (int m = 0; m < 15; m++)
	for (int o = 0; o < 16; o++)
	{
		getc = fgetc(read);

		if (getc != '\n')
			grille[m][o] = getc - '0';
	}

	for (int id = 0; id < 2; id++)
	{
		do{
			abs = generer_bornes(0, 15);
			ord = generer_bornes(0, 15);

		} while (grille[ord][abs] != 2);

		grille[ord][abs] = 6;
	}

	for (int i = 0; i < 2; ++i)
	{
		do{
			absu = generer_bornes(0, 15);
			ordu = generer_bornes(0, 15);

		} while (grille[ordu][absu] != 4);

		grille[ordu][absu] = 7;
	}

	for (int i = 0; i < 2; ++i)
	{
		do{
			absd = generer_bornes(0, 15);
			ordd = generer_bornes(0, 15);

		} while (grille[ordd][absd] != 1);

		grille[ordd][absd] = 9;
	}
	for (int i = 0; i < 1; ++i)
	{
		do{
			abs = generer_bornes(0, 15);
			ord = generer_bornes(0, 15);

		} while (grille[ord][abs] != 1);

		grille[ord][abs] = 10;
	}
	fclose(read);

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (grille[i][j] == 6)
			{
				r->n[0][n].x = j;
				r->n[0][n].y = i;
				r->posBonus[0][n].x = j * 44 + 5, r->posBonus[0][n].y = i * 22 + 5;
				n++;
			}
			else if (grille[i][j] == 7)
			{
				r->n[1][p].x = j;
				r->n[1][p].y = i;
				r->posBonus[1][p].x = j * 44 + 5, r->posBonus[1][p].y = i * 22 + 5;
				p++;
			}
			else if (grille[i][j] == 9)
			{
				r->n[2][m].x = j;
				r->n[2][m].y = i;
				r->posBonus[2][m].x = j * 44 + 5, r->posBonus[2][m].y = i * 22 + 5;
				m++;
			}
			else if (grille[i][j] == 10)
			{
				r->n[3][k].x = j;
				r->n[3][k].y = i;
				r->posBonus[3][k].x = j * 44 + 5, r->posBonus[3][k].y = i * 22 + 5;
				k++;
			}
		}
	}

	r->nbBonus[0] = n + 1;
	r->nbBonus[1] = p + 1;
	r->nbBonus[2] = m + 1;
	r->nbBonus[3] = k + 1;

}


int editeur(char grille[16][16], obj *ball)
{
	int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0, objetChoisi = 1;

	int n = 0, p = 0;
	FILE* read = NULL;
	SDL_Rect position;
	SDL_Event event;

	ball->bleu = IMG_Load("bleu.png");
	ball->surface = IMG_Load("ball.jpg");
	ball->vert = IMG_Load("vert.bmp");
	ball->rouge = IMG_Load("Rouge.png");
	ball->blanc = IMG_Load("Blanc.png");
	ball->gris = IMG_Load("Gris.png");
	ball->plot = IMG_Load("plot.png");

	read = fopen("level1.txt", "r");

	if (!read)
		return NULL;

	char getc = 0;

	for (int m = 0; m < 15; m++)
	for (int o = 0; o < 16; o++)
	{
		getc = fgetc(read);

		if (getc != '\n')
			grille[m][o] = getc - '0';
	}

	fclose(read);
	while (continuer)

	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			continuer = 0;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_KP1:
				objetChoisi = 1;
				break;
			case SDLK_KP2:
				objetChoisi = 2;
				break;
			case SDLK_KP3:
				objetChoisi = 3;
				break;
			case SDLK_KP4:
				objetChoisi = 4;
				break;
			case SDLK_KP5:
				objetChoisi = 5;
				break;
			case SDLK_KP6:
				objetChoisi = 8;
				break;
			case SDLK_s:
				sauvegarderNiveau(grille);
				break;
			case SDLK_ESCAPE:
				continuer = 0;
				break;
			}

			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				grille[event.button.y / 22][event.button.x / 44] = objetChoisi;
				clicGaucheEnCours = 1; 
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				grille[event.button.y / 22][event.button.x / 44] = 0;
				clicDroitEnCours = 1;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				clicGaucheEnCours = 0;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				clicDroitEnCours = 0;
			break;

		case SDL_MOUSEMOTION:
			if (clicGaucheEnCours) 
			{
				grille[event.motion.y / 22][event.motion.x / 44] = objetChoisi;
			}
			else if (clicDroitEnCours)
			{
				grille[event.motion.y / 22][event.motion.x / 44] = 0;
			}
			break;

		}

		SDL_FillRect(ball->ecran, NULL, SDL_MapRGB(ball->ecran->format, 0, 0, 0));

		for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
		{
			position.x = i * 44;
			position.y = j * 22;

			switch (grille[j][i])
			{
			case 1:
				SDL_BlitSurface(ball->vert, NULL, ball->ecran, &position);
				break;
			case 2:
				SDL_BlitSurface(ball->bleu, NULL, ball->ecran, &position);
				break;
			case 3:
				SDL_BlitSurface(ball->rouge, NULL, ball->ecran, &position);
				break;
			case 4:
				SDL_BlitSurface(ball->blanc, NULL, ball->ecran, &position);
				break;
				break;
			case 5:
				SDL_BlitSurface(ball->gris, NULL, ball->ecran, &position);
				break;
			case 8:
				SDL_BlitSurface(ball->plot, NULL, ball->ecran, &position);
				break;
			}
			
		}
		SDL_Flip(ball->ecran);
		SDL_Delay(20);
	}
}

int sauvegarderNiveau(char niveau[16][16])
{
	FILE* fichier = NULL;
	int i = 0, j = 0;

	fichier = fopen("level1.txt", "w");
	if (fichier == NULL)
		return 0;

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			fprintf(fichier, "%d", niveau[i][j]);
		}
		fputc('\n', fichier);
	}

	fclose(fichier);
	return 1;
}

int rand_a_b(int a, int b)
{
	return rand() % (b - a) + a;
}

int generer_bornes(int min, int max)
{
	static int rand_is_seeded = 0;
	if (!rand_is_seeded)
	{
		srand(time(NULL));
		rand_is_seeded = 1;
	}
	return rand() % (max - min + 1) + min;
}

