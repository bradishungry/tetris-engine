#include "SaveManager.h"
#include <Windows.h>
#include <stdio.h>

internal_static StackAllocator save_allocator(Megabytes(10));

SaveManager::SaveManager(){}
SaveManager::~SaveManager(){}

void SaveManager::startUp(){
    OutputDebugStringA("Load scores");
    loadScores();
    ASSERT(scoreList->score);
    ASSERT(scoreCount != 0);
}

void SaveManager::shutDown(){
    save_allocator.clear();
}

void SaveManager::saveScore(u32 score) {

}

void SaveManager::loadScores() {
    scoreCount = 10;
    ScoreData* s_pos = (ScoreData*)save_allocator.alloc(sizeof(ScoreData) * scoreCount);
    scoreList = s_pos;
    for (int i = 0; i < scoreCount; i += 1) {
        char buffer_player[256];
        sprintf_s(buffer_player, "Player %d", i + 1);
        strcpy_s(scoreList->playerName, buffer_player);

        char buffer_date[256];
        sprintf_s(buffer_date, "7/%d/22", (i + 1) * 2 - 1);
        strcpy_s(scoreList->date, buffer_date);
        scoreList->score = 200000 - (i * 100);
        scoreList += 1;
    }
    //Testing allocations
    scoreList -= 1;
    save_allocator.freeToMarker((StackAllocator::Marker)scoreList);
    char buffer_player[256];
    sprintf_s(buffer_player, "Player X");
    strcpy_s(scoreList->playerName, buffer_player);

    char buffer_date[256];
    sprintf_s(buffer_date, "7/XX/22");
    strcpy_s(scoreList->date, buffer_date);
    scoreList->score = 30000;
    scoreList += 1;

    scoreList = s_pos;
}
