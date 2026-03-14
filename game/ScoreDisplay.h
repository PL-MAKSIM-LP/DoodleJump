#pragma once

class ScoreDisplay {
public:
    void setScore(int s) { score = s; }
    int  getScore() const { return score; }

    // TODO: render digit sprites once a suitable spritesheet is identified
    void render() {}

private:
    int score = 0;
};
