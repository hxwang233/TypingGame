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

/*雨滴结构体*/
typedef struct {
    int x,y,r;  //位置及半径大小
    int speed;  //下落速度
    int alpha;  //透明度
}Spot;

/*分数结构体*/
typedef struct {
    int point;  //分数
    SDL_Color color;  //颜色
}Score;

/*气球结构体*/
typedef struct {
    int letter;  //字母
    int x,y,h,w; //位置及大小
    int holeRowNo,holeColNo;  //所处洞口横坐标、纵坐标
    int speed;  //速度
    int score;  //分数
    int alpha;  //透明度
    SDL_Texture* text;  //字母Texture
    bool moveUp,state;  //状态量
    Uint32 lastUpdate;  //上一次更新时间
}Balloon;

void drawLetter(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* text); //画字母
void drawBalloon(Balloon* balloon, SDL_Renderer* rend, SDL_Texture* image, SDL_Texture* text); //画气球
void drawScore(Score socre, SDL_Renderer* rend,  TTF_Font* font); //画分数
void balloonMove(Balloon* balloon);  //气球移动
void reSet(Balloon* balloon, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor); //重置气球
void initBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor); //初始气球
void updateBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, SDL_Texture* image, TTF_Font* font, SDL_Color fontColor); //更新气球
void destoryLetterTexture(Balloon* balloons, int n); //摧毁所有字体Texture
TTF_Font* loadTTF(char* file); //加载ttf
void openAudio(); //加载mixer
void gunShotPlay();  //播放枪声
void errorSoundPlay(); //播放错误提示音
void sort(Balloon* balloons, int n); //气球排序 按洞口远近
int getCurrentAmount(Uint32 startTime, Uint32 currentTime);  //获取当前应该出现的气球数目 与时间有关
void initRaindrops(Spot* spots, int n);  //初始化雨点
void rainMove(Spot* spot);  //雨点移动
void updateRaindrops(Spot* spots, int n, SDL_Renderer* rend, SDL_Texture* circle); //更新雨点
int select(SDL_Rect rect);  //选择操作
void gameStart();  //开始游戏

