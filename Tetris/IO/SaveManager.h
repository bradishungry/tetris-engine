#pragma once
#include "definitions.h"
#include "StackAllocator.h"

typedef struct _score {
    u32 score;
    char playerName[256];
    char date[256];
} ScoreData;

class SaveManager {
public:
    SaveManager();
    ~SaveManager();
    void startUp();
    void shutDown();
    void saveScore(uint32_t score);
    void loadScores();
    ScoreData *scoreList;
    u16 scoreCount;
};