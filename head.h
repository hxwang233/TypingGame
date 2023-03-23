#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL_mixer.h>
#include <string.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 400
#define BALLOON_MAX_AMOUNT 5
#define RAINDROP_AMOUNT 100
#define ALPHA_WEEKEN 3
#define MAX_ALPHA 255
#define MIN_ALPHA 155
#define ONE_SECOND 1000
#define CUT_POINTS 10

/*��νṹ��*/
typedef struct {
    int x,y,r;  //λ�ü��뾶��С
    int speed;  //�����ٶ�
    int alpha;  //͸����
}Spot;

/*�����ṹ��*/
typedef struct {
    int point;  //����
    SDL_Color color;  //��ɫ
}Score;

/*����ṹ��*/
typedef struct {
    int letter;  //��ĸ
    int x,y,h,w; //λ�ü���С
    int holeRowNo,holeColNo;  //�������ں����ꡢ������
    int speed;  //�ٶ�
    int score;  //����
    int alpha;  //͸����
    SDL_Texture* text;  //��ĸTexture
    bool moveUp,state;  //״̬��
    Uint32 lastUpdate;  //��һ�θ���ʱ��
}Balloon;

void drawLetter(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* text); //����ĸ
void drawBalloon(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* image, SDL_Texture* text); //������
void drawScore(Score socre, SDL_Renderer* rend,  TTF_Font* font); //������
void balloonMove(Balloon* balloon);  //�����ƶ�
void reSet(Balloon* balloon, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor); //��������
void initBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor); //��ʼ����
void updateBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, SDL_Texture* image, TTF_Font* font, SDL_Color fontColor); //��������
void destoryLetterTexture(Balloon* balloons, int n); //�ݻ���������Texture
TTF_Font* loadTTF(char* file); //����ttf
void openAudio(); //����mixer
void gunShotPlay();  //����ǹ��
void errorSoundPlay(); //���Ŵ�����ʾ��
void sort(Balloon* balloons, int n); //�������� ������Զ��
int getCurrentAmount(Uint32 startTime, Uint32 currentTime);  //��ȡ��ǰӦ�ó��ֵ�������Ŀ ��ʱ���й�
void initRaindrops(Spot* spots, int n);  //��ʼ�����
void rainMove(Spot* spot);  //����ƶ�
void updateRaindrops(Spot* spots, int n, SDL_Renderer* rend, SDL_Texture* circle); //�������
int select(SDL_Rect rect);  //ѡ�����
void gameStart();  //��ʼ��Ϸ

