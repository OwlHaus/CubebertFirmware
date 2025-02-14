#ifndef RCSP_H
#define RCSP_H

#include "claw.h"
#include "fipc.h"

#include <queue>
#include <string>

// Enum for grip step
enum MoveOrientation {
    Latitude,
    Longitude,
    Unset,
};

// Enum for Grip
enum Grip {
    North,
    South,
    East,
    West,
    NorthSouth,
    EastWest,
};

// Enum for grip state
enum GripState {
    VerticalClosed,
    HorizontalClosed,
    AllClosed,
    AllOpen,
};

// Struct for storing the current move
struct Move {
    MoveOrientation moveOrientation;
    
    GripState northGrip;
    GripState southGrip;
    GripState eastGrip;
    GripState westGrip;

    int32_t northGripRotation;
    int32_t southGripRotation;
    int32_t eastGripRotation;
    int32_t westGripRotation;
};

struct NewMove {
    Move* move;
    bool pushMove;
};

class RCSP {
public:
    RCSP(FIPC* fipc);
    std::queue<Move*> *parseSolve(std::queue<std::string> solve);
    void printMoveQueue(std::queue<Move*> *queue);

private:
    Claw *m_claw;
    int32_t m_angle;
    FIPC* m_fipc;

    NewMove calculateRotation(Move* previousMove, Grip grip, int multiplier);
    void calculateGripState(Move *curMove, Move *previousMove, Grip grip);
    std::queue<Move*> *parseMove(Move *previousMove, std::queue<std::string> solve, std::queue<Move*> *moves);
};

#endif // RCSP_H
