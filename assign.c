#include "head.h"
// 245 153 153
void gameStart(){
    SDL_Rect rectWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Color fontColor = {0, 0, 0, 0};
    //SDL_Rect rect = {0, 0, 50, 100};
    Score score = {0, {0,0,0}};  //初始分数 0 颜色{0,0, 0)
    int currentAmount = 1;   //当前气球数量
    Uint32 startTime;   //开始时间
    SDL_Init(SDL_INIT_EVERYTHING); //SDL初始所有
    SDL_Window* window = SDL_CreateWindow("Typing Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,//窗口出现位置系统决定
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(rend);  //清空初始
    SDL_Surface* bgSurface = IMG_Load("image/map.jpg");  //地图
    SDL_Surface* imageSurface = IMG_Load("image/balloon.jpg");   //气球
    SDL_Surface* whiteSurface = IMG_Load("image/white.jpg");     //纯白色
    SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 168, 168, 168));  //去除气球图片某种颜色
    SDL_Texture* image = SDL_CreateTextureFromSurface(rend, imageSurface);  //转换为可以贴在rend上的texture
    SDL_Texture* bg = SDL_CreateTextureFromSurface(rend, bgSurface);
    SDL_Texture* circle = SDL_CreateTextureFromSurface(rend, whiteSurface);
    SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(bg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(bg, 100);  //透明度为100
    SDL_SetTextureColorMod(circle, 166, 255, 255);  //设置雨点颜色

    TTF_Font* font = loadTTF("Bell Gothic Black BT.ttf");  //加载字体
    openAudio();  //加载声音
    Balloon balloons[BALLOON_MAX_AMOUNT];  //定义气球数组
    int holes[3][3];  //定义地洞数组
    int letters[26];  //定义字母数组
    Spot spots[RAINDROP_AMOUNT];  //定义雨点数组

    initBalloons(balloons, BALLOON_MAX_AMOUNT, holes, letters, rend, font, fontColor);  //初始化气球
    initRaindrops(spots, RAINDROP_AMOUNT);  //初始化雨点

    bool quit = false;  //退出标识
    SDL_Event event;
    startTime = SDL_GetTicks();  //获取开始时间
    while (quit == false) {
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){  // 点击窗口X退出
                quit = true;
            }
            else if (event.type == SDL_KEYUP){  //键盘事件
                int i;
                for (i=0; i<currentAmount; i++){ //击中判断
                    if ((balloons[i].state == true) && (event.key.keysym.sym == balloons[i].letter + 32)){ // 气球升起 && 按键值等于气球上字母
                        for (; balloons[i].alpha!=0; balloons[i].alpha-=ALPHA_WEEKEN){  //逐渐消失被击中气球
                            SDL_RenderCopy(rend, bg, NULL, &rectWindow);
                            for(int j=0; j<currentAmount; j++){ //画所有气球
                                drawBalloon(&balloons[j], rend, image, balloons[j].text);
                            }
                            gunShotPlay();  //播放枪声
                            SDL_RenderPresent(rend);
                        }
                        score.point += balloons[i].score;  //加分
                        SDL_Color white = {0, 0, 0};
                        score.color = white;         //分数设置为白色
                        balloons[i].state  = false;  //气球不可用
                        break;
                    }
                }
                if (i == currentAmount){  //未击中
                    errorSoundPlay();  //播放错误提示声
                    SDL_Color red = {255, 0, 0};
                    score.point -= CUT_POINTS;   //减分
                    score.color = red;  //分数设置为红色
                }
            }
        }
        //SDL_RenderClear(rend);
        SDL_RenderCopy(rend, bg, NULL, &rectWindow);
        updateRaindrops(spots, RAINDROP_AMOUNT, rend, circle);  //更新雨点
        drawScore(score, rend, font);  //更新分数
        Uint32 currentTime = SDL_GetTicks();
        currentAmount = getCurrentAmount(startTime, currentTime);  //获取当前应出现气球数目
        sort(balloons, currentAmount);  //气球数组排序，洞口位于后面的先画，避免重叠
        updateBalloons(balloons, currentAmount, holes, letters, rend, image, font, fontColor);  //更新气球
        SDL_RenderPresent(rend);

        if (score.point < 0) {  //如果分数小于0
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, bg, NULL, &rectWindow);
            drawScore(score, rend, font);  //更新分数
            SDL_Rect rect = {20, WINDOW_HEIGHT / 5, 250, 200};
            SDL_Surface* gameOverSurface = IMG_Load("image/GameOver.png");
            SDL_SetColorKey(gameOverSurface, SDL_TRUE, SDL_MapRGB(gameOverSurface->format, 239, 220, 211));
            SDL_Texture* gameOver  = SDL_CreateTextureFromSurface(rend, gameOverSurface);
            SDL_RenderCopy(rend, gameOver, NULL, &rect);
            SDL_RenderPresent(rend);
            quit = true;  //退出
            int flag = select(rect);  //选择下一步 继续或退出  （-1 - 重新开始 & 其余退出）
            /*释放资源*/
            destoryLetterTexture(balloons, currentAmount); //销毁字体Texture
            TTF_CloseFont(font);  //关闭ttf
            TTF_Quit();  //退出
            Mix_ChannelFinished(2);  //关闭声音通道
            Mix_CloseAudio();  //关闭Mixer
            SDL_DestroyTexture(gameOver);
            SDL_FreeSurface(gameOverSurface);
            SDL_DestroyTexture(bg);
            SDL_DestroyTexture(image);
            SDL_DestroyTexture(circle);
            SDL_FreeSurface(bgSurface);
            SDL_FreeSurface(imageSurface);
            SDL_FreeSurface(whiteSurface);
            SDL_DestroyRenderer(rend);
            SDL_DestroyWindow(window);  //摧毁窗口
            SDL_Quit();  //退出
            if (flag == -1){
                gameStart();  //重新开始游戏
            }
        }
        SDL_Delay(15);
    }
}

/*初始化气球*/
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
        while (letters[letter - 65] != 0){  //去重
            letter = rand() % (90 - 65 + 1) + 65;
        }
        letters[letter - 65] = 1;
        balloons[i].letter = letter;

        int n = rand() % 3;  // 0~2
        int m = rand() % 3;  // 0~2
        while (holes[n][m] != 0){ //去重
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

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, &(balloons[i].letter), fontColor);  //生成字母
        SDL_Texture* text  = SDL_CreateTextureFromSurface(rend, textSurface);
        balloons[i].text   = text;
        balloons[i].moveUp = true;
        balloons[i].speed  = rand() % 3 + 1;  //速度 1~3
        balloons[i].score  = balloons[i].speed;  //分值等于速度
        balloons[i].alpha  = MAX_ALPHA;
        balloons[i].state  = false;   //设置不可用
        SDL_FreeSurface(textSurface); //释放当前字体的SDL_Surface
    }
}

/*更新气球*/
void updateBalloons(Balloon* balloons, int n, int holes[3][3], int letters[26], SDL_Renderer* rend, SDL_Texture* image, TTF_Font* font, SDL_Color fontColor){
    for (int i=0; i<n; i++){
        balloonMove(&balloons[i]);  //气球坐标更新
        drawBalloon(&balloons[i], rend, image, balloons[i].text);  //画气球
        if (balloons[i].state == false){  //如果当前气球不可用，重置气球
           reSet(&balloons[i], holes, letters, rend, font, fontColor);
        }
    }
}

/*重置气球*/
void reSet(Balloon* balloon, int holes[3][3], int letters[26], SDL_Renderer* rend, TTF_Font* font, SDL_Color fontColor){
    srand((unsigned)time(NULL));
    letters[balloon->letter - 65] = 0;
    int letter = rand() % (90 - 65 + 1) + 65;
    while (letters[letter - 65] != 0){  //去重
        letter = rand() % (90 - 65 + 1) + 65;
    }
    letters[letter - 65] = 1;
    balloon->letter = letter;

    holes[balloon->holeRowNo][balloon->holeColNo] = 0;
    int n = rand() % 3;
    int m = rand() % 3;
    while (holes[n][m] != 0){ //去重
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
    SDL_FreeSurface(textSurface); //释放当前字体的SDL_Surface
}

/*销毁字体Texture*/
void destoryLetterTexture(Balloon* balloons, int n){
    for (int i=0; i<n; i++){ //摧毁所有字体纹理
        SDL_DestroyTexture(balloons[i].text);
    }
}

/*初始化雨滴*/
void initRaindrops(Spot* spots, int n){
    srand((unsigned)time(NULL));
    for (int i=0; i<n; i++){
        spots[i].x = rand() % WINDOW_WIDTH;
        spots[i].y = rand() % WINDOW_HEIGHT / 2;  //上半屏幕
        spots[i].r = rand() % 3 + 1;
        spots[i].speed = rand() % 2 + 1;
        spots[i].alpha = rand() % MAX_ALPHA;
    }
}

/*更新雨滴*/
void updateRaindrops(Spot* spots, int n, SDL_Renderer* rend, SDL_Texture* circle){
    for (int i=0; i<n; i++){
        drawSpot(&spots[i], rend, circle);
        rainMove(&spots[i]);
    }
}

/*选择操作*/
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
