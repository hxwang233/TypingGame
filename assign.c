#include "head.h"
// 245 153 153
void gameStart(){
    SDL_Rect rectWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Color fontColor = {0, 0, 0, 0};
    //SDL_Rect rect = {0, 0, 50, 100};
    Score score = {0, {0,0,0}};  //��ʼ���� 0 ��ɫ{0,0, 0)
    int currentAmount = 1;   //��ǰ��������
    Uint32 startTime;   //��ʼʱ��
    SDL_Init(SDL_INIT_EVERYTHING); //SDL��ʼ����
    SDL_Window* window = SDL_CreateWindow("Typing Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,//���ڳ���λ��ϵͳ����
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(rend);  //��ճ�ʼ
    SDL_Surface* bgSurface = IMG_Load("image/map.jpg");  //��ͼ
    SDL_Surface* imageSurface = IMG_Load("image/balloon.jpg");   //����
    SDL_Surface* whiteSurface = IMG_Load("image/white.jpg");     //����ɫ
    SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 168, 168, 168));  //ȥ������ͼƬĳ����ɫ
    SDL_Texture* image = SDL_CreateTextureFromSurface(rend, imageSurface);  //ת��Ϊ��������rend�ϵ�texture
    SDL_Texture* bg = SDL_CreateTextureFromSurface(rend, bgSurface);
    SDL_Texture* circle = SDL_CreateTextureFromSurface(rend, whiteSurface);
    SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(bg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(bg, 100);  //͸����Ϊ100
    SDL_SetTextureColorMod(circle, 166, 255, 255);  //���������ɫ

    TTF_Font* font = loadTTF("Bell Gothic Black BT.ttf");  //��������
    openAudio();  //��������
    Balloon balloons[BALLOON_MAX_AMOUNT];  //������������
    int holes[3][3];  //����ض�����
    int letters[26];  //������ĸ����
    Spot spots[RAINDROP_AMOUNT];  //�����������

    initBalloons(balloons, BALLOON_MAX_AMOUNT, holes, letters, rend, font, fontColor);  //��ʼ������
    initRaindrops(spots, RAINDROP_AMOUNT);  //��ʼ�����

    bool quit = false;  //�˳���ʶ
    SDL_Event event;
    startTime = SDL_GetTicks();  //��ȡ��ʼʱ��
    while (quit == false) {
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){  // �������X�˳�
                quit = true;
            }
            else if (event.type == SDL_KEYUP){  //�����¼�
                int i;
                for (i=0; i<currentAmount; i++){ //�����ж�
                    if ((balloons[i].state == true) && (event.key.keysym.sym == balloons[i].letter + 32)){ // �������� && ����ֵ������������ĸ
                        for (; balloons[i].alpha!=0; balloons[i].alpha-=ALPHA_WEEKEN){  //����ʧ����������
                            SDL_RenderCopy(rend, bg, NULL, &rectWindow);
                            for(int j=0; j<currentAmount; j++){ //����������
                                drawBalloon(&balloons[j], rend, image, balloons[j].text);
                            }
                            gunShotPlay();  //����ǹ��
                            SDL_RenderPresent(rend);
                        }
                        score.point += balloons[i].score;  //�ӷ�
                        SDL_Color white = {0, 0, 0};
                        score.color = white;         //��������Ϊ��ɫ
                        balloons[i].state  = false;  //���򲻿���
                        break;
                    }
                }
                if (i == currentAmount){  //δ����
                    errorSoundPlay();  //���Ŵ�����ʾ��
                    SDL_Color red = {255, 0, 0};
                    score.point -= CUT_POINTS;   //����
                    score.color = red;  //��������Ϊ��ɫ
                }
            }
        }
        //SDL_RenderClear(rend);
        SDL_RenderCopy(rend, bg, NULL, &rectWindow);
        updateRaindrops(spots, RAINDROP_AMOUNT, rend, circle);  //�������
        drawScore(score, rend, font);  //���·���
        Uint32 currentTime = SDL_GetTicks();
        currentAmount = getCurrentAmount(startTime, currentTime);  //��ȡ��ǰӦ����������Ŀ
        sort(balloons, currentAmount);  //�����������򣬶���λ�ں�����Ȼ��������ص�
        updateBalloons(balloons, currentAmount, holes, letters, rend, image, font, fontColor);  //��������
        SDL_RenderPresent(rend);

        if (score.point < 0) {  //�������С��0
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, bg, NULL, &rectWindow);
            drawScore(score, rend, font);  //���·���
            SDL_Rect rect = {20, WINDOW_HEIGHT / 5, 250, 200};
            SDL_Surface* gameOverSurface = IMG_Load("image/GameOver.png");
            SDL_SetColorKey(gameOverSurface, SDL_TRUE, SDL_MapRGB(gameOverSurface->format, 239, 220, 211));
            SDL_Texture* gameOver  = SDL_CreateTextureFromSurface(rend, gameOverSurface);
            SDL_RenderCopy(rend, gameOver, NULL, &rect);
            SDL_RenderPresent(rend);
            quit = true;  //�˳�
            int flag = select(rect);  //ѡ����һ�� �������˳�  ��-1 - ���¿�ʼ & �����˳���
            /*�ͷ���Դ*/
            destoryLetterTexture(balloons, currentAmount); //��������Texture
            TTF_CloseFont(font);  //�ر�ttf
            TTF_Quit();  //�˳�
            Mix_ChannelFinished(2);  //�ر�����ͨ��
            Mix_CloseAudio();  //�ر�Mixer
            SDL_DestroyTexture(gameOver);
            SDL_FreeSurface(gameOverSurface);
            SDL_DestroyTexture(bg);
            SDL_DestroyTexture(image);
            SDL_DestroyTexture(circle);
            SDL_FreeSurface(bgSurface);
            SDL_FreeSurface(imageSurface);
            SDL_FreeSurface(whiteSurface);
            SDL_DestroyRenderer(rend);
            SDL_DestroyWindow(window);  //�ݻٴ���
            SDL_Quit();  //�˳�
            if (flag == -1){
                gameStart();  //���¿�ʼ��Ϸ
            }
        }
        SDL_Delay(15);
    }
}

/*��ʼ������*/
void initBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor){
    srand((unsigned)time(NULL));
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            holes[i][j] = 0;
        }
    }

    for (int i=0; i<26; i++){
        letters[i] = 0;
    }

    for (int i=0; i<n; i++){
        int letter = rand() % (90 - 65 + 1) + 65;  // A~Z
        while (letters[letter - 65] != 0){  //ȥ��
            letter = rand() % (90 - 65 + 1) + 65;
        }
        letters[letter - 65] = 1;
        balloons[i].letter = letter;

        int n = rand() % 3;  // 0~2
        int m = rand() % 3;  // 0~2
        while (holes[n][m] != 0){ //ȥ��
            n = rand() % 3;
            m = rand() % 3;
        }
        holes[n][m] = 1;
        balloons[i].holeRowNo = n;
        balloons[i].holeColNo = m;

        balloons[i].x = 42 + n * 85;
        balloons[i].y = 240 + m * 60;
        balloons[i].w = 51;  //balloons[i].x = 212;   //42 127 212  /85
        balloons[i].h = 0;  //balloons[i].y = 240;   //240 300 360  /60

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, &(balloons[i].letter), fontColor);  //������ĸ
        SDL_Texture* text  = SDL_CreateTextureFromSurface(rend, textSurface);
        balloons[i].text   = text;
        balloons[i].moveUp = true;
        balloons[i].speed  = rand() % 3 + 1;  //�ٶ� 1~3
        balloons[i].score  = balloons[i].speed;  //��ֵ�����ٶ�
        balloons[i].alpha  = MAX_ALPHA;
        balloons[i].state  = false;   //���ò�����
        SDL_FreeSurface(textSurface); //�ͷŵ�ǰ�����SDL_Surface
    }
}

/*��������*/
void updateBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, SDL_Texture* image, TTF_Font* font, SDL_Color fontColor){
    for (int i=0; i<n; i++){
        balloonMove(&balloons[i]);  //�����������
        drawBalloon(&balloons[i], rend, image, balloons[i].text);  //������
        if (balloons[i].state == false){  //�����ǰ���򲻿��ã���������
           reSet(&balloons[i], holes, letters, rend, font, fontColor);
        }
    }
}

/*��������*/
void reSet(Balloon* balloon, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor){
    srand((unsigned)time(NULL));
    letters[balloon->letter - 65] = 0;
    int letter = rand() % (90 - 65 + 1) + 65;
    while (letters[letter - 65] != 0){  //ȥ��
        letter = rand() % (90 - 65 + 1) + 65;
    }
    letters[letter - 65] = 1;
    balloon->letter = letter;

    holes[balloon->holeRowNo][balloon->holeColNo] = 0;
    int n = rand() % 3;
    int m = rand() % 3;
    while (holes[n][m] != 0){ //ȥ��
        n = rand() % 3;
        m = rand() % 3;
    }
    holes[n][m] = 1;
    balloon->holeRowNo = n;
    balloon->holeColNo = m;
    balloon->x = 42 + n * 85;
    balloon->y = 250 + m * 60;
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, &(balloon->letter), fontColor);
    SDL_Texture* text  = SDL_CreateTextureFromSurface(rend, textSurface);
    balloon->text   = text;
    balloon->state  = true;
    SDL_FreeSurface(textSurface); //�ͷŵ�ǰ�����SDL_Surface
}

/*��������Texture*/
void destoryLetterTexture(Balloon* balloons, int n){
    for (int i=0; i<n; i++){ //�ݻ�������������
        SDL_DestroyTexture(balloons[i].text);
    }
}

/*��ʼ�����*/
void initRaindrops(Spot* spots, int n){
    srand((unsigned)time(NULL));
    for (int i=0; i<n; i++){
        spots[i].x = rand() % WINDOW_WIDTH;
        spots[i].y = rand() % WINDOW_HEIGHT / 2;  //�ϰ���Ļ
        spots[i].r = rand() % 3 + 1;
        spots[i].speed = rand() % 2 + 1;
        spots[i].alpha = rand() % MAX_ALPHA;
    }
}

/*�������*/
void updateRaindrops(Spot* spots, int n, SDL_Renderer* rend, SDL_Texture* circle){
    for (int i=0; i<n; i++){
        drawSpot(&spots[i], rend, circle);
        rainMove(&spots[i]);
    }
}

/*ѡ�����*/
int select(SDL_Rect rect){
    SDL_Event e;
    int flag = 1;
    while (flag == 1){
        while (SDL_PollEvent(&e)){
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(rect.x + rect.w / 5 * 3 < e.button.x && e.button.x < rect.x + rect.w / 5 *4 && rect.y + rect.h / 6 * 5 < e.button.y && e.button.y < rect.y + rect.h){
                    flag = 0;
                }
                if (rect.x + rect.w / 5 * 1  < e.button.x && e.button.x < rect.x + rect.x + rect.w / 5 * 1.5 && rect.y + rect.h / 6 * 5 < e.button.y && e.button.y < rect.y + rect.h){
                    flag = -1;
                }
            }
        }
    }
    return flag;
}
