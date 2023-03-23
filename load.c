#include "head.h"

/*����ttf*/
TTF_Font* loadTTF(char* file){
    TTF_Init();
	TTF_Font* font = TTF_OpenFont(file, 30);
	if (font == NULL){
		printf("TTF_OpenFont: Open Bell Gothic Black BT.ttf %s\n", TTF_GetError());
		return;
	}
	return font;
}

/*����mixer*/
void openAudio(){
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

/*����ǹ��*/
void gunShotPlay(){
    Mix_Music* sound = Mix_LoadMUS("sound/gunShot.wav");
    Mix_PlayMusic(sound, 1);
}

/*���Ŵ�����ʾ��*/
void errorSoundPlay(){
    Mix_Music* sound = Mix_LoadMUS("sound/alert.wav");
    Mix_PlayMusic(sound, 1);
}
