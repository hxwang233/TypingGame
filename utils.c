#include "head.h"

/*气球排序  按洞口远近*/
void sort(Balloon* balloons, int n){
    int i, j;
    Balloon tmp;
    for (i = 0; i < n; i++){
        int min = i;
        for (j = i + 1; j < n; j++)
        {
            if (balloons[j].holeColNo < balloons[min].holeColNo)
            {
                tmp = balloons[min];
                balloons[min] = balloons[j];
                balloons[j] = tmp;
            }
        }
    }
}

/*获取当前气球数量*/
int getCurrentAmount(Uint32 startTime, Uint32 currentTime){
    int currentAmount;
    if (0 < currentTime - startTime && currentTime - startTime < 15*ONE_SECOND){  //游戏开始15秒内 1只气球
        currentAmount = 1;
    }
    else if (15 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 40 * ONE_SECOND){ //游戏开始15~40秒内 2只气球
        currentAmount = 2;
    }
    else if (40 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 70 * ONE_SECOND){ //游戏开始40~70秒内 3只气球
        currentAmount = 3;
    }
    else if (70 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 120 * ONE_SECOND){ //游戏开始70~120秒内 4只气球
        currentAmount = 4;
    }
    else {  //游戏开始120秒后 所有气球升起
        currentAmount = BALLOON_MAX_AMOUNT;
    }
    return currentAmount;
}
