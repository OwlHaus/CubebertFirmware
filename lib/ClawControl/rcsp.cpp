#include "rcsp.h"

#include <cstdio>
#include <format>

RCSP::RCSP(FIPC* fipc) {
    m_fipc = fipc;
}

// Function to calculate the rotation
NewMove RCSP::calculateRotation(Move* previousMove, Grip grip, int multiplier) {
    NewMove newMove{
        nullptr,
        true,
    };
    MoveOrientation orientation = grip == Grip::East || grip == Grip::West ? MoveOrientation::Longitude : MoveOrientation::Latitude;
    if (previousMove->moveOrientation == orientation && grip != Grip::NorthSouth && grip != Grip::EastWest) {
        newMove.move = previousMove;
        newMove.pushMove = false;
    } else {
        newMove.move = new Move{
            orientation,
            previousMove->northGrip,
            previousMove->southGrip,
            previousMove->eastGrip,
            previousMove->westGrip,
            0,
            0,
            0,
            0,
        };
    }

    switch(grip) {
        case Grip::North:
            newMove.move->northGripRotation += 90 * multiplier;
            if (newMove.move->northGripRotation == 270) {
                newMove.move->northGripRotation = -90;
            } else if (newMove.move->northGripRotation == -270) {
                newMove.move->northGripRotation = 90;
            }
            break;
        case Grip::South:
            newMove.move->southGripRotation += 90 * multiplier;
            if (newMove.move->southGripRotation == 270) {
                newMove.move->southGripRotation = -90;
            } else if (newMove.move->southGripRotation == -270) {
                newMove.move->southGripRotation = 90;
            }
            break;
        case Grip::East:
            newMove.move->eastGripRotation += 90 * multiplier;
            if (newMove.move->eastGripRotation == 270) {
                newMove.move->eastGripRotation = -90;
            } else if (newMove.move->eastGripRotation == -270) {
                newMove.move->eastGripRotation = 90;
            }
            break;
        case Grip::West:
            newMove.move->westGripRotation += 90 * multiplier;
            if (newMove.move->westGripRotation == 270) {
                newMove.move->westGripRotation = -90;
            } else if (newMove.move->westGripRotation == -270) {
                newMove.move->westGripRotation = 90;
            }
            break;
        case Grip::NorthSouth:
            newMove.move->eastGripRotation += 90 * multiplier;
            newMove.move->westGripRotation += -90 * multiplier;
            break;
        case Grip::EastWest:
            newMove.move->northGripRotation += 90 * multiplier;
            newMove.move->southGripRotation += -90 * multiplier;
            break;
    }

    return newMove;
}

// Function to calculate the grip state
void RCSP::calculateGripState(Move *curMove, Move *previousMove, Grip grip) {
    if (grip == Grip::NorthSouth) {
        curMove->northGrip = GripState::AllOpen;
        curMove->southGrip = GripState::AllOpen;
        curMove->eastGrip = GripState::AllClosed;
        curMove->westGrip = GripState::AllClosed;
    } else if (grip == Grip::EastWest) {
        curMove->northGrip = GripState::AllClosed;
        curMove->southGrip = GripState::AllClosed;
        curMove->eastGrip = GripState::AllOpen;
        curMove->westGrip = GripState::AllOpen;
    } else {
        curMove->northGrip = (curMove->northGripRotation / 90) % 2 ? GripState::HorizontalClosed : GripState::VerticalClosed;
        curMove->southGrip = (curMove->southGripRotation / 90) % 2 ? GripState::HorizontalClosed : GripState::VerticalClosed;
        curMove->eastGrip = (curMove->eastGripRotation / 90) % 2 ? GripState::HorizontalClosed : GripState::VerticalClosed;
        curMove->westGrip = (curMove->westGripRotation / 90) % 2 ? GripState::HorizontalClosed : GripState::VerticalClosed;
    }
}

// Recursive Function to parse a move
std::queue<Move*> *RCSP::parseMove(Move *previousMove, std::queue<std::string> solve, std::queue<Move*> *moves) {
    // If the solve is empty, return the moves
    if (solve.empty()) {
        return moves;
    }

    // Get the next move
    std::string move = solve.front();
    solve.pop();

    int multiplier = move.length() == 1 ? 1 : move[1] == '\'' ? -1 : 2;

    Grip grip;
    switch(move[0]) {
        case 'R': grip = Grip::East; break;
        case 'L': grip = Grip::West; break;
        case 'F': grip = Grip::South; break;
        case 'B': grip = Grip::North; break;
        case 'X': grip = Grip::EastWest; break;
        case 'Z': grip = Grip::NorthSouth; break;
        default:
            printf("Invalid move: %s\n", move.c_str());
            return moves;
    }

    NewMove newMove = calculateRotation(previousMove, grip, multiplier);
    calculateGripState(newMove.move, previousMove, grip);

    // Push the move to the moves queue
    if (newMove.pushMove) {
        moves->push(newMove.move);
    }

    return parseMove(newMove.move, solve, moves);
}

// Function to parse the solve
std::queue<Move*> *RCSP::parseSolve(std::queue<std::string> solve) {
    // Create a queue to store the moves
    std::queue<Move*> *moves = new std::queue<Move*>();

    return parseMove(new Move{
        MoveOrientation::Unset,
        GripState::AllOpen,
        GripState::AllOpen, 
        GripState::AllOpen,
        GripState::AllOpen,
        0,
        0,
        0,
        0,
    }, solve, moves);
}

void RCSP::printMoveQueue(std::queue<Move*> *queue) {
    // lambda to turn gripstate into string
    auto gripStateToString = [](GripState gripState) {
        switch(gripState) {
            case GripState::VerticalClosed:
                return "Vertical Closed";
            case GripState::HorizontalClosed:
                return "Horizontal Closed";
            case GripState::AllClosed:
                return "All Closed";
            case GripState::AllOpen:
                return "All Open";
            default:
                return "Unknown";
        }
    };

    // Print the moves
    while(!queue->empty()) {
        Move *move = queue->front();
        m_fipc->puts(std::format("Move:\r\n\
\tnorthGrip: {} \r\n\
\tsouthGrip: {} \r\n\
\teastGrip: {} \r\n\
\twestGrip: {} \r\n\
\tnorthGripRotation: {} \r\n\
\tsouthGripRotaton: {} \r\n\
\teastGripRotation: {} \r\n\
\twestGripRotation: {} \r\n\r\n", gripStateToString(move->northGrip), gripStateToString(move->southGrip), gripStateToString(move->eastGrip), gripStateToString(move->westGrip), move->northGripRotation, move->southGripRotation, move->eastGripRotation, move->westGripRotation));
        queue->pop();
        delete move;
    }
}

// int main() {
//     RCSP rcsp;

//     // Declare our solve
//     std::string solve = "L' Z L' B' R F' B X F2 R' B R2 B2 R2 F2 L2 F2 Z R2 B2 F2 L2";
//     printf("Solve: %s\n", solve.c_str());

//     // Create a queue to store the moves
//     std::queue<std::string> moves;
//     while(solve.length() != 0) {
//         moves.push(solve.substr(0, solve.find(' ')));
//         if (solve.find(' ') == std::string::npos) {
//             break;
//         }
//         solve = solve.substr(solve.find(' ') + 1);
//     }
//     printf("Moves: %ld\n", moves.size());

//     // Parse the solve
//     std::queue<Move*> *parsedSolve = rcsp.parseSolve(moves);
//     printf("Parsed Moves: %ld\n", parsedSolve->size());

//     printMoveQueue(parsedSolve);
// }