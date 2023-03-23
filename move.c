#include "head.h"


void rainMove(Spot* spot){
    spot->y = spot->y + spot->speed;
    spot->x = spot->x - spot->speed;
    if (spot->x <= 0){
        spot->x = WINDOW_WIDTH;
        return;
    }
    if (spot->y >= WINDOW_HEIGHT / 2){
        spot->y = 0;
        return;
    }
    return;
}

/*�����������*/
void balloonMove(Balloon* balloon){
    //�������������� ͣ��1��
    if (balloon->moveUp == false && balloon->h >= 100){
        if (SDL_GetTicks() - balloon->lastUpdate <= 1000){
            return ;
        }
        else{
            balloon->h = 100;
        }
    }
    //������
    if (balloon->moveUp == true){
        balloon->alpha  = MAX_ALPHA;
        balloon->y = balloon->y - balloon->speed;
        if (balloon->h <= 100){
            balloon->h = balloon->h + balloon->speed;
        }
        if (balloon->h > 100){
            balloon->lastUpdate = SDL_GetTicks();
            balloon->moveUp = false;
        }
    }
    //���½�
    else {
        balloon->y = balloon->y + balloon->speed;
        if (balloon->h >= 5){
            balloon->h = balloon->h - balloon->speed;
        }
        if (balloon->h < 5){
            balloon->moveUp = true;
            balloon->state = false;
        }
    }
}
