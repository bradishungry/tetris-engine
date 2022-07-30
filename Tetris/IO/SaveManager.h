#pragma once
#include <cstdint>

typedef struct _score {
    uint32_t score;
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
    uint16_t scoreCount;
};