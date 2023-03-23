#include "head.h"

/*»­×ÖÄ¸*/
void drawLetter(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* text){
    SDL_Rect rect;
    rect.w  = 20;
    rect.h  = 30;
    rect.x  = balloon->x + balloon->w / 2 - rect.w / 2;
    rect.y  = balloon->y + balloon->h / 2 - rect.h / 2 - 5;
	SDL_SetTextureAlphaMod(text, balloon->alpha);
	SDL_RenderCopy(rend, text, NULL, &rect);
}

/*»­·ÖÊý*/
void drawScore(Score socre, SDL_Renderer* rend,  TTF_Font* font){
    char str1[25] = "Score: ";
    char str2[10];
    itoa(socre.point, str2, 10);  //×Ö·û´®Æ´½Ó
    strcat(str1, str2);
    SDL_Rect rect = {WINDOW_WIDTH - 80, 0, 70, 20};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, str1, socre.color);
    SDL_Texture* text  = SDL_CreateTextureFromSurface(rend, textSurface);
	SDL_RenderCopy(rend, text, NULL, &rect);
}

/*»­ÆøÇò*/
void drawBalloon(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* image, SDL_Texture* text){
    if (balloon->state == true){
        SDL_Rect showRect;
        showRect.x = 0;
        showRect.y = 0;
        showRect.w = 51;
        showRect.h = balloon->h;

        SDL_Rect rect;
        rect.x = balloon->x;
        rect.y = balloon->y;
        rect.w = showRect.w;
        rect.h = balloon->h;
        SDL_SetTextureAlphaMod(image, balloon->alpha);
        SDL_RenderCopy(rend, image, &showRect, &rect);
        if (balloon->h > 80 && balloon->h <120){  //ÆøÇò³öÏÖÁË4/5 ¿ªÊ¼»­×ÖÄ¸
            drawLetter(balloon, rend, text);
        }
    }
}

/*»­ÓêµÎ*/
void drawSpot(Spot* spot, SDL_Renderer* rend, SDL_Texture* image){
    SDL_Rect rect;
    rect.x = spot->x;
    rect.y = spot->y;
    rect.w = spot->r;
    rect.h = spot->r;
    SDL_SetTextureAlphaMod(image, spot->alpha);
    SDL_RenderCopy(rend, image, NULL, &rect);
}
