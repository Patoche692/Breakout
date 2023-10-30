#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL_ttf.h>
#include "Header.h"

#define TICK_INTERVAL       5
#define PAD_MAX_SPEED       20.
#define BALL_SPEED          10.
#define BALL_MAX_SPEED      12
#define BALL_START_X        -0.5
#define BALL_START_Y        1
#define COEFF_DX            0.1
#define COEFF_SPEED			0.1

char grille[16][16];
enum{ DROITE, GAUCHE, HAUT };

int main(void)
{
	int continuer = 1;
	SDL_Event event, event2;
	obj rond;
	matrice mat;
	SDL_Surface *menu = NULL, *txtmenu = NULL, *bypat = NULL, *spacetc = NULL;
	SDL_Rect position;
	SDL_Color couleurBleue = { 0, 0, 255 }, couleurNoire = { 255, 0, 0 }, couleurVerte = { 0, 255, 0 }, couleurBlanche = { 255, 255, 255 };
	TTF_Font *police = NULL, *policed = NULL, *policeb = NULL;
	rond.indEvent = 1;

	rond.lvl = 0, rond.ind = 0;
	TTF_Init();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	police = TTF_OpenFont("arial.ttf", 40);
	policed = TTF_OpenFont("arial.ttf", 30);
	policeb = TTF_OpenFont("BRUSHSCI.ttf", 25);

	SDL_WM_SetIcon(IMG_Load("bleu.png"), NULL);

	rond.ecran = SDL_SetVideoMode(15 * 44, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Casse-Brique", NULL);

	menu = IMG_Load("menu.png");
	txtmenu = TTF_RenderText_Blended(police, "Brick-Breaker", couleurNoire);
	bypat = TTF_RenderText_Blended(policed, "By patoche", couleurBleue);
	spacetc = TTF_RenderText_Blended(policeb, "Press space to play and e for editor", couleurBlanche);

	while (continuer)
	{
		if (rond.indEvent)
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
				case SDLK_SPACE:
					rond.nbBallR = 3;
					create_matrice(grille, &rond), rond.indEvent = 0;
					SDL_FillRect(rond.ecran, NULL, SDL_MapRGB(rond.ecran->format, 0, 0, 0));
					jouer(&rond);
					break;
				case SDLK_e:
					editeur(grille, &rond);
					break;
				case SDLK_ESCAPE:
					continuer = 0;
					break;
				}
				break;
			}
			SDL_FillRect(rond.ecran, NULL, SDL_MapRGB(rond.ecran->format, 0, 0, 0));

			position.x = rond.ecran->w / 2 - menu->w / 2, position.y = 50;

			SDL_BlitSurface(menu, NULL, rond.ecran, &position);

			position.x = rond.ecran->w / 2 - txtmenu->w / 2, position.y = 300;
			SDL_BlitSurface(txtmenu, NULL, rond.ecran, &position);

			position.x = rond.ecran->w / 2 - bypat->w / 2, position.y = 350;
			SDL_BlitSurface(bypat, NULL, rond.ecran, &position);

			position.x = rond.ecran->w / 2 - spacetc->w / 2, position.y = 410;
			SDL_BlitSurface(spacetc, NULL, rond.ecran, &position);

			SDL_Flip(rond.ecran);
		}
		else
		{
			create_matrice(grille, &rond), jouer(&rond);
		}

	}
	SDL_Quit();
	TTF_Quit();
}


void jouer(obj *rond)
{
	rond->indDef = 0;
	int dpl;
	if (rond->lvl >= 3)
		dpl = 3;
	else 
		dpl = 2;

	rond->ind = 0;
	rond->indPlanche = 0;
	for (int u = 0; u < 4; ++u)
	for (int g = 0; g < 4; ++g)
	{
		rond->indBonus[u][g] = 0;
	}
	for (int u = 0; u < 9; ++u)
		rond->indBall[u] = 0;

	rond->indBall[0] = 1;

	rond->continuer = 1, rond->score = 0;
	SDL_Event event, event2;
	obj planche, rond2;
	SDL_Rect position, pos;
	SDL_Surface *rect, *rondBig = NULL, *txt = NULL, *txtNbB = NULL, *txtlvla = NULL, *menu = NULL;
	TTF_Font *police = NULL;
	SDL_Color couleurBlanche = { 255, 255, 255 }, couleurNoire = { 0, 0, 0 };
	char texte[20] = "", txtnbb[20] = "", txtlvl[20]="";

	rond->bleu = IMG_Load("bleu.png");
	rond->ball[0] = IMG_Load("rond.jpg");
	rond->ball[1] = IMG_Load("soleil.png");
	rond->surface = rond->ball[0];
	rond->vert = IMG_Load("vert.bmp");
	rond->rouge = IMG_Load("Rouge.png");
	rond->blanc = IMG_Load("Blanc.png");
	rond->gris = IMG_Load("Gris.png");
	rond->plot = IMG_Load("plot3.bmp");
	rond->bonus[0] = IMG_Load("Bonus1.png");
	rond->bonus[1] = IMG_Load("Bonus3.png");
	rond->bonus[2] = IMG_Load("Bonus4.png");
	rond->bonus[3] = IMG_Load("Bonus2.png");
	planche.surface = IMG_Load("planche.png");
	rondBig = IMG_Load("rondBig.png");

	sprintf(txtlvl, "LEVEL : %d", rond->lvl + 1);
	sprintf(txtnbb, "LIVES : %d", rond->nbBallR);
	sprintf(texte, "SCORE : %d", rond->score);

	police = TTF_OpenFont("arial.ttf", 20);
	txtlvla = TTF_RenderText_Shaded(police, txtlvl, couleurBlanche, couleurNoire);
	txt = TTF_RenderText_Shaded(police, texte, couleurBlanche, couleurNoire);
	txtNbB = TTF_RenderText_Shaded(police, txtnbb, couleurBlanche, couleurNoire);

	planche.pos[0].x = rond->ecran->w / 2 - planche.surface->w / 2;
	planche.pos[0].y = 446;
	rond->pos[0].x = rond->ecran->w / 2 - rond->surface->w / 2; 
	rond->pos[0].y = 300;
	 
	nbAlSpeed(rond);

	rond->posPlanche.x = planche.pos[0].x;
	rond->posPlanche.y = planche.pos[0].y;

	Ball_ensureSpeed(rond);

	rond->timer = SDL_AddTimer(21, bouger_rond, rond);

	while (rond->continuer)
	{

		blitter_surfaces(rond, planche.surface, txtNbB, txt, txtlvla);
		SDL_Flip(rond->ecran);

		SDL_WaitEvent(&event);
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_SPACE)
			{
				rond->ind = 1, rond->continuer = 0;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				rond->indEvent = 1, rond->continuer = 0, SDL_RemoveTimer(rond->timer), rond->lvl = 0;
			}
		}
		else if (event.type == SDL_QUIT)
			rond->indEvent = 1, rond->continuer = 0, SDL_RemoveTimer(rond->timer);
	}

	if (rond->indEvent != 1)
	rond->continuer = 1;

	while (rond->continuer)
	{

		if (rond->indPlanche)
			planche.surface = IMG_Load("BigPlanche.png");

		sprintf(txtlvl, "LEVEL : %d", rond->lvl + 1);
		SDL_FreeSurface(txtlvla);
		txtlvla = TTF_RenderText_Shaded(police, txtlvl, couleurBlanche, couleurNoire);

		sprintf(txtnbb, "LIVES : %d", rond->nbBallR);
		SDL_FreeSurface(txtNbB);
		txtNbB = TTF_RenderText_Shaded(police, txtnbb, couleurBlanche, couleurNoire);

		sprintf(texte, "SCORE : %d", rond->score);
		SDL_FreeSurface(txt);
		txt = TTF_RenderText_Shaded(police, texte, couleurBlanche, couleurNoire);

		if (il_ny_a_plus_de_balles(rond))
		{
			rond->nbBallR--;
			rond->indBall[0] = 1;
			rond->pos[0].x = 200, rond->pos[0].y = 300;
			nbAlSpeed(rond);
			rond->coeff[0] = 1;
			Ball_ensureSpeed(rond);
			rond->ind = 0;
			planche.pos[0].x = rond->ecran->w / 2 - planche.surface->w / 2;
		}

		int i, j;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
				rond->indEvent = 1;
				rond->continuer = 0;
				SDL_RemoveTimer(rond->timer);
				break;
		case SDL_KEYDOWN:
			rond->ind = 1;
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (position_valide(&planche.pos[0].x, planche.surface->w))
				{
					planche.pos[0].x += dpl;
					rond->posPlanche.x = planche.pos[0].x, rond2.posPlanche.x = planche.pos[0].x;
				}
				break;

			case SDLK_LEFT:
				if (position_valide(&planche.pos[0].x, planche.surface->w))
				{
					planche.pos[0].x -= dpl;
					rond->posPlanche.x = planche.pos[0].x;
				}
				break;
			case SDLK_ESCAPE:
				rond->indEvent = 1;
				rond->continuer = 0;
				SDL_RemoveTimer(rond->timer), rond->lvl = 0;
				break;
			}
			break;
		case SDL_KEYUP:
			planche.dir = HAUT;
			break;
		}

		SDL_FillRect(rond->ecran, NULL, SDL_MapRGB(rond->ecran->format, 0, 0, 0));

		for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
		{
			position.x = i * 44;
			position.y = j * 22;

			switch (grille[j][i])
			{
			case 1:
			case 9:
			case 10:
				SDL_BlitSurface(rond->vert, NULL, rond->ecran, &position);
				break;
			case 2:
				SDL_BlitSurface(rond->bleu, NULL, rond->ecran, &position);
				break;
			case 3:
				SDL_BlitSurface(rond->rouge, NULL, rond->ecran, &position);
				break;
			case 4:
			case 7:
				SDL_BlitSurface(rond->blanc, NULL, rond->ecran, &position);
				break;
				break;
			case 5:
				SDL_BlitSurface(rond->gris, NULL, rond->ecran, &position);
				break;
			case 6:
				SDL_BlitSurface(rond->bleu, NULL, rond->ecran, &position);
				break;
			case 8:
				SDL_BlitSurface(rond->plot, NULL, rond->ecran, &position);
				break;
			}

		}
		for (int h = 0; h < 4; ++h)
		{
			for (int u = 0; u < 4; ++u)
			{
				if (rond->indBonus[u][h] == 1)
				{
					SDL_BlitSurface(rond->bonus[u], NULL, rond->ecran, &rond->posBonus[u][h]);
				}
			}
		}

		SDL_BlitSurface(planche.surface, NULL, rond->ecran, &planche.pos[0]);

		for (int k = 0; k < 9; ++k)
		{
			if (rond->indBall[k])
			{
				SDL_BlitSurface(rond->surface, NULL, rond->ecran, &rond->pos[k]);
			}
		}
		pos.x = rond->ecran->w - 150, pos.y = 460;
		SDL_BlitSurface(txt, NULL, rond->ecran, &pos);

		pos.x = 5 , pos.y = 460;
		SDL_BlitSurface(txtNbB, NULL, rond->ecran, &pos);

		pos.x = rond->ecran->w / 2 - txtlvla->w / 2;
		SDL_BlitSurface(txtlvla, NULL, rond->ecran, &pos);

		if (rond->nbBallR == 0)
		{
			rond->continuer = 0;
			rond->nbBallR = 3;
			rond->lvl = 0;
			SDL_RemoveTimer(rond->timer);
		}
		SDL_Flip(rond->ecran);
		SDL_Delay(2);
	}
}


void initialiser_grille(obj *r)
{
	int n = 0, u = 0, p = 0;
	r->lvl++;
	int i, j;

	if (r->lvl == 2)
	{
		for (i = 0; i < 20; i++)
		for (j = 0; j < 30; j++)
		{
			grille[i][j] = 0;
		}

		for (i = 3; i < 7; ++i)
		for (j = 1; j < 2; ++j)
			grille[i][j] = 7;

	}

	else if (r->lvl == 1)
	{
		for (i = 0; i < 20; i++)
		for (j = 0; j < 30; j++)
		{
			grille[i][j] = 0;
		}

		for (i = 2; i < 13; i += 2)
		for (j = 2; j < 3; ++j)
			grille[i][0] = 1;

		for (i = 3; i < 13; i += 2)
		for (j = 2; j < 3; ++j)
			grille[i][0] = 2;

		for (i = 1; i < 10; i += 12)
		for (j = 1; j < 9; j += 2)
			grille[i][j] = 7;

		for (i = 1; i < 10; i += 12)
		for (j = 2; j < 9; j += 2)
			grille[i][j] = 6;

		for (i = 4; i < 11; ++i)
			grille[i][7] = 1;

		grille[3][6] = 1, grille[11][6] = 1, grille[5][3] = 5, grille[8][3] = 5;

		for (i = 2; i < 13; i += 2)
			grille[i][9] = 3;

		for (i = 3; i < 13; i += 2)
			grille[i][9] = 4;

	}
	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; ++j)
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
		}

	}
	r->nbBonus[0] = n + 1;
	r->nbBonus[1] = p + 1;
}


Uint32 bouger_rond(Uint32 interval, void *param)
{
	int nBallAct[9] = { 0 }, nbBallAct = 0, num = 0, nBallPut = 1, ind = 0, speedx = 2, speedy = -2;
	int y = 0;
	float w = 0, coeffa = 0;
	int h = 22;
	SDL_Rect position;
	obj *r = param;
	int x, n = 0;

	if (r->indDef)
	{
		r->tempsDef--;
		if (r->tempsDef <= 0)
		{
			r->indDef = 0;
			r->surface = r->ball[0];
		}
	}

	if (r->ind)
	{
		for (int u = 0; u < 4; ++u)
		{
			for (int k = 0; k < 4; ++k)
			{
				if (r->indBonus[u][k] == 1)
				{
					r->posBonus[u][k].y += 3;
					if (r->posBonus[u][k].x + 30 >= r->posPlanche.x &&
						r->posBonus[u][k].x <= r->posPlanche.x + 70 &&
						r->posBonus[u][k].y + 15 >= 446 &&
						r->posBonus[u][k].y <= r->posPlanche.y + 14)
					{
						r->indBonus[u][k] = 0;

						if (u == 0)
							r->indPlanche = 1;

						else if (u == 1)
						{


							for (int y = 0; y < 9; ++y)
							{
								if (r->indBall[y] == 1)
								{
									nbBallAct++;
									nBallAct[nbBallAct - 1] = y;
								}
							}
							if (nbBallAct == 1)
							{
								while (nBallPut <= 2)
								{
									if (r->indBall[ind] == 0)
									{
										r->indBall[ind] = 1;

										r->pos[ind].x = r->pos[nBallAct[0]].x, r->pos[ind].y = r->pos[nBallAct[0]].y, r->xspeed[ind] = speedx, r->yspeed[ind] = -3;
										Ball_ensureSpeed(r);

										nBallPut++;
										speedx *= -1;
									}
									ind++;
								}
							}
							else if (nbBallAct == 2)
							{
								while (nBallPut <= 4)
								{
									if (r->indBall[ind] == 0)
									{
										r->indBall[ind] = 1;

										r->pos[ind].x = r->pos[nBallAct[n]].x, r->pos[ind].y = r->pos[nBallAct[n]].y, r->xspeed[ind] = speedx, r->yspeed[ind] = -3;
										Ball_ensureSpeed(r);

										nBallPut++;
										speedx *= -1;

										if (nBallPut == 2)
											n++;
									}
									ind++;
								}
							}
							else if (nbBallAct == 3)
							{
								for (int u = 0; u < 9; ++u)
								{
									if (r->indBall[u] == 0)
									{
										r->indBall[u] = 1;

										r->pos[u].x = r->pos[nBallAct[num]].x, r->pos[u].y = r->pos[nBallAct[num]].y, r->xspeed[u] = speedx, r->yspeed[u] = -3;
										Ball_ensureSpeed(r);
										num++;
										speedx *= -1;
									}
								}
							}
						}
						else if (u == 2)
						{
							r->indDef = 1, r->tempsDef = 200, r->surface = r->ball[1];
						}
						else if (u == 3)
						{
							r->nbBallR++;
						}
						r->score += 200;
					}
					else if (r->posBonus[u][k].y >= 480)
						r->indBonus[u][k] = 0;
				}
			}
		}

		for (int k = 0; k < 9; ++k)
		{
			if (r->indBall[k] == 1)

			{
				r->pos[k].x += r->xspeed[k] * r->coeff[k];
				r->pos[k].y += r->yspeed[k] * r->coeff[k];

				if (r->pos[k].x <= 0)
				{
					if (r->xspeed[k] <= 0);
					r->xspeed[k] *= -1.;
				}

				if (r->pos[k].y + 10 >= 480 && r->pos[k].y + 10 <= 480 + r->yspeed[k] * r->coeff[k])
				{
					r->indBall[k] = 0;

				}

				if (r->pos[k].y <= 0)
				{
					if (r->yspeed[k] <= 0)
						r->yspeed[k] *= -1.;
				}

				if (r->pos[k].x + r->surface->w >= 15 * 44)
				{
					r->xspeed[k] *= -1.;
					r->pos[k].x = 15 * 44 - r->surface->w;
				}

				if (r->indPlanche)
					w = 100, coeffa = 0.07;
				else
					w = 70, coeffa = 0.1;

				if (r->pos[k].x + r->surface->w >= r->posPlanche.x &&
					r->pos[k].x <= r->posPlanche.x + w &&
					r->pos[k].y + r->surface->h >= 446 &&
					r->pos[k].y <= r->posPlanche.y + r->yspeed[k] * r->coeff[k])
				{
					double dx = r->posPlanche.x + w / 2 - (r->pos[k].x + r->surface->w / 2);
					if (r->yspeed[k] >= 0)
						r->yspeed[k] *= -1.;

					r->xspeed[k] = -dx * coeffa + 1 * COEFF_SPEED;
					if (r->xspeed[k] > BALL_MAX_SPEED)
						r->xspeed[k] = BALL_MAX_SPEED;
					if (r->xspeed[k] < -BALL_MAX_SPEED)
						r->xspeed[k] = -BALL_MAX_SPEED;

					Ball_ensureSpeed(r);

					if (r->score >= 20)
					r->score -= 20;

				}
			}
		}
		for (int k = 0; k < 9; k++)
		{
			if (r->indBall[k])

			{
				int col = r->pos[k].x / 44;
				int lin = r->pos[k].y / 22;

				for (int i = 0; i < 15; ++i)
				for (int j = col - 1; j <= col + 1; ++j)
				{

					if (grille[i][j] == 1 || grille[i][j] == 2 || grille[i][j] == 3 || grille[i][j] == 4 ||grille[i][j] == 5 ||
						grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 8 || grille[i][j] == 9 || grille[i][j] == 10)
					{
						if (grille[i][j] == 5)
							h = 44;
						else
							h = 22;

						position.x = j * 44, position.y = i * 22;

						if (r->pos[k].x  + r->surface->w >= position.x &&
							r->pos[k].x <= position.x + 44 &&
							r->pos[k].y <= position.y + h &&
							r->pos[k].y >= position.y + h + (r->yspeed[k] * r->coeff[k]))
						{
							if (!r->indDef)
							{
								if (grille[i][j] == 3)
									grille[i][j] = 1, r->yspeed[k] *= -1., r->pos[k].y = position.y + h + 1;
								else if (grille[i][j] == 5)
									grille[i][j] = 4, grille[i + 1][j] = 4, r->yspeed[k] *= -1., r->pos[k].y = position.y + h + 1;
								else if (grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 9 || grille[i][j] == 10)
								{
									for (int u = 0; u < 4; u++)
									{
										for (int w = 0; w < 4; w++)
										{
											if (r->n[u][w].x == j && r->n[u][w].y == i)
												grille[i][j] = 0, r->yspeed[k] *= -1, r->pos[k].y = position.y + h + 1, r->indBonus[u][w] = 1;
										}
									}
								}

								else if (grille[i][j] == 8)
								{
									r->yspeed[k] *= -1, r->pos[k].y = position.y + h + 1;
								}
								else if (grille[i][j] == 1 || grille[i][j] == 2 || grille[i][j] == 4)
									grille[i][j] = 0, r->yspeed[k] *= -1, r->pos[k].y = position.y + h + 1;

								r->score += 100;
							}
							else
							{
								if (grille[i][j] != 8)
									grille[i][j] = 0, r->score += 100;
								else 
									r->yspeed[k] *= -1, r->pos[k].y = position.y + h + 1;
							}
						}
						else if (r->pos[k].x + r->surface->w >= position.x &&
							r->pos[k].x  <= position.x + 44 &&
							r->pos[k].y + r->surface->h >= position.y &&
							r->pos[k].y + r->surface->h <= position.y + (r->yspeed[k] * r->coeff[k]))
						{
							if (!r->indDef)
							{
								if (grille[i][j] == 3)
									grille[i][j] = 1, r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1;
								else if (grille[i][j] == 5)
									grille[i][j] = 4, grille[i + 1][j] = 4, r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1;
								else if (grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 9 || grille[i][j] == 10)
								{
									for (int w = 0; w < 4; w++)
									{
										for (int u = 0; u < 4; ++u)
										{
											if (r->n[u][w].x == j && r->n[u][w].y == i)
												grille[i][j] = 0, r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1, r->indBonus[u][w] = 1;
										}
									}
								}
								else if (grille[i][j] == 8)
								{
									r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1;
								}
								else
									grille[i][j] = 0, r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1;

								r->score += 100;
							}
							else
							{
								if (grille[i][j] != 8)
									grille[i][j] = 0, r->score += 100;
								else
									r->yspeed[k] *= -1, r->pos[k].y = position.y - r->surface->h - 1;
							}
						}
						else if (r->pos[k].y + r->surface->h >= position.y &&
							r->pos[k].y <= position.y + h &&
							r->pos[k].x + r->surface->w >= position.x  &&
							r->pos[k].x + r->surface->w <= position.x + (r->xspeed[k] * r->coeff[k]))
						{
							if (!r->indDef)
							{
								if (grille[i][j] == 3)
									grille[i][j] = 1, r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1;
								else if (grille[i][j] == 5)
									grille[i][j] = 4, grille[i + 1][j] = 4, r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1;
								else if (grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 9 || grille[i][j] == 10)
								{
									for (int w = 0; w < 4; w++)
									{
										for (int u = 0; u < 4; ++u)
										{
											if (r->n[u][w].x == j && r->n[u][w].y == i)
												grille[i][j] = 0, r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1, r->indBonus[u][w] = 1;
										}
									}
								}
								else if (grille[i][j] == 8)
								{
									r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1;
								}
								else
									grille[i][j] = 0, r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1;

								r->score += 100;
							}
							else
							{
								if (grille[i][j] != 8)
									grille[i][j] = 0, r->score += 100;
								else
									r->xspeed[k] *= -1, r->pos[k].x = position.x - r->surface->w - 1;
							}
						}
						else if (r->pos[k].y + r->surface->h >= position.y &&
							r->pos[k].y <= position.y + h &&
							r->pos[k].x >= position.x + 44 + (r->xspeed[k] * r->coeff[k]) &&
							r->pos[k].x <= position.x + 44)
						{
							if (!r->indDef)
							{
								if (grille[i][j] == 3)
									grille[i][j] = 1, r->xspeed[k] *= -1, r->pos[k].x = position.x + 45;
								else if (grille[i][j] == 5)
									grille[i][j] = 4, grille[i + 1][j] = 4, r->xspeed[k] *= -1, r->pos[k].x = position.x + 45;
								else if (grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 9 || grille[i][j] == 10)
								{
									for (int w = 0; w < 4; w++)
									{
										for (int u = 0; u < 4; ++u)
										{
											if (r->n[u][w].x == j && r->n[u][w].y == i)
												grille[i][j] = 0, r->xspeed[k] *= -1, r->pos[k].x = position.x + 45, r->indBonus[u][w] = 1;
										}
									}
								}
								else if (grille[i][j] == 8)
								{
									r->xspeed[k] *= -1, r->pos[k].x = position.x + 45;
								}
								else
									grille[i][j] = 0, r->xspeed[k] *= -1, r->pos[k].x = position.x + 45;

								r->score += 100;
							}
							else
							{
								if (grille[i][j] != 8)
									grille[i][j] = 0, r->score += 100;
								else
									r->xspeed[k] *= -1, r->pos[k].x = position.x + 45;
							}
						}
					}
				}
			}
		}

		if (gagne())
		{
			SDL_Delay(500);
			r->lvl++;
			r->nbBallR++;
			r->continuer = 0;
			SDL_RemoveTimer(r->timer);
		}

	}
	return interval;
}


int position_valide(Sint16 *px, int w)
{
	if (*px <= 0)
	{
		++*px;
		return 0;
	}
	else if (*px + w >= 15 * 44)//pk 70?-->longueur de la planche trou de balle!!!!
	{
		--*px;
		return 0;
	}

	return 1;
}


int gagne()
{
	int i, j;
	for (i = 0; i < 15; ++i)
	for (j = 0; j < 15; ++j)
	{
		if (grille[i][j] == 1 || grille[i][j] == 2 || grille[i][j] == 3 || grille[i][j] == 4 ||
			grille[i][j] == 5 || grille[i][j] == 6 || grille[i][j] == 7 || grille[i][j] == 9)
			return 0;
	}
		return 1;


}

int Ball_ensureSpeed(obj *ball)
{
	for (int k = 0; k < 9; ++k)
	{
		if (ball->indBall[k])
		{
			double speed = ball->coeff[k] * sqrt(ball->xspeed[k] * ball->xspeed[k] +
				ball->yspeed[k] * ball->yspeed[k]);
			ball->coeff[k] *= (BALL_SPEED / speed);
		}
	}
}


void blitter_surfaces( obj *rond, SDL_Surface *planche, SDL_Surface *nbB, SDL_Surface *score, SDL_Surface *lvl)
{
	SDL_Rect position, pos;
	int j, i;

	SDL_FillRect(rond->ecran, NULL, SDL_MapRGB(rond->ecran->format, 0, 0, 0));
	for (i = 0; i < 15; i++)
	for (j = 0; j < 15; j++)
	{
		position.x = j * 44;
		position.y = i * 22;

		switch (grille[i][j])
		{
		case 1:
		case 9:
		case 10:
			SDL_BlitSurface(rond->vert, NULL, rond->ecran, &position);
			break;
		case 2:
			SDL_BlitSurface(rond->bleu, NULL, rond->ecran, &position);
			break;
		case 3:
			SDL_BlitSurface(rond->rouge, NULL, rond->ecran, &position);
			break;
		case 4:
		case 7:
			SDL_BlitSurface(rond->blanc, NULL, rond->ecran, &position);
			break;
			break;
		case 5:
			SDL_BlitSurface(rond->gris, NULL, rond->ecran, &position);
			break;
		case 6:
			SDL_BlitSurface(rond->bleu, NULL, rond->ecran, &position);
			break;
		case 8 :
			SDL_BlitSurface(rond->plot, NULL, rond->ecran, &position);
			break;
		}

	}
	pos.x = rond->ecran->w / 2 - planche->w / 2 , pos.y = 446;
	SDL_BlitSurface(planche, NULL, rond->ecran, &pos);
	SDL_BlitSurface(rond->surface, NULL, rond->ecran, &rond->pos);
	pos.x = rond->ecran->w - 150, pos.y = 460;
	SDL_BlitSurface(score, NULL, rond->ecran, &pos);

	pos.x = 5, pos.y = 460;
	SDL_BlitSurface(nbB, NULL, rond->ecran, &pos);

	pos.x = rond->ecran->w / 2 - lvl->w / 2;
	SDL_BlitSurface(lvl, NULL, rond->ecran, &pos);
}


int il_ny_a_plus_de_balles(obj *r)
{
	for (int k = 0; k < 9; ++k)
	if (r->indBall[k] == 1)
		return 0;

	return 1;

}

int nbAlSpeed(obj *ball)
{
	do{
		ball->xspeed[0] = generer_bornes(-3, 3);
		ball->yspeed[0] = generer_bornes(-3, 3);
	} while (ball->xspeed[0] * ball->xspeed[0] + ball->yspeed[0] * ball->yspeed[0] != 13);


}