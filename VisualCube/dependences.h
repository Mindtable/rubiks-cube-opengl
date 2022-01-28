//
// Created by MTBLE on 17.01.2022.
//

#ifndef GEN_ALGO_DEPENDENCES_H
#define GEN_ALGO_DEPENDENCES_H

#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <functional>
#include <random>
#include <ctime>
#include <list>
#include <fstream>
#include <cstdlib>

enum Color {
    Green = 0,
    Orange = 1,
    Red = 2,
    White = 3,
    Yellow = 4,
    Blue = 5
};

enum Side {
    Front = 0,
    Left = 1,
    Right = 2,
    Top = 3,
    Bottom = 4,
    Back = 5
};

namespace Moves {
    enum Move {
        U = 0, Ur, U2, D, Dr, D2, R, Rr, R2, L, Lr, L2, F, Fr, F2, B, Br, B2, x, xr, x2, y, yr, y2, z, zr, z2, M, Mr, M2
    };
}

#endif //GEN_ALGO_DEPENDENCES_H
