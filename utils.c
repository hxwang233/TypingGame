#include "head.h"

/*��������  ������Զ��*/
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

/*��ȡ��ǰ��������*/
int getCurrentAmount(Uint32 startTime, Uint32 currentTime){
    int currentAmount;
    if (0 < currentTime - startTime && currentTime - startTime < 15*ONE_SECOND){  //��Ϸ��ʼ15���� 1ֻ����
        currentAmount = 1;
    }
    else if (15 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 40 * ONE_SECOND){ //��Ϸ��ʼ15~40���� 2ֻ����
        currentAmount = 2;
    }
    else if (40 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 70 * ONE_SECOND){ //��Ϸ��ʼ40~70���� 3ֻ����
        currentAmount = 3;
    }
    else if (70 * ONE_SECOND <= currentTime - startTime && currentTime - startTime < 120 * ONE_SECOND){ //��Ϸ��ʼ70~120���� 4ֻ����
        currentAmount = 4;
    }
    else {  //��Ϸ��ʼ120��� ������������
        currentAmount = BALLOON_MAX_AMOUNT;
    }
    return currentAmount;
}
