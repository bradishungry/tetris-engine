#include "SaveManager.h"
#include <Windows.h>
#include <stdio.h>

SaveManager::SaveManager(){}
SaveManager::~SaveManager(){}

void SaveManager::startUp(){
    OutputDebugStringA("Load scores");
    loadScores();
}

void SaveManager::shutDown(){
    free(scoreList);
}

void SaveManager::saveScore(uint32_t score) {

}

void SaveManager::loadScores() {
    scoreCount = 10;
    ScoreData* s_pos = (ScoreData*)malloc(sizeof(ScoreData) * scoreCount);
    scoreList = s_pos;
    for (int i = 0; i < scoreCount; i += 1) {
        char buffer_player[256];
        sprintf_s(buffer_player, "Player %d", i + 1);
        strcpy_s(scoreList->playerName,  buffer_player);

        char buffer_date[256];
        sprintf_s(buffer_date, "7/%d/22", (i + 1) * 2 - 1);
        strcpy_s(scoreList->date, buffer_date);
        scoreList->score = 200000 - (i * 100);
        scoreList += 1;
    }
    scoreList = s_pos;
}
