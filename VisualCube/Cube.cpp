//
// Created by MTBLE on 22.01.2022.
//

#include "Cube.h"

template<>
void Cube::make_move<Moves::U>() {
    make_f(Top);
    swap_x(0);
};

template<>
void Cube::make_move<Moves::Ur>() {
    make_fr(Top);
    swap_x(0, true);
};

template<>
void Cube::make_move<Moves::U2>() {
    make_f2(Top);
    swap_x(0);
    swap_x(0);
};

template<>
void Cube::make_move<Moves::D>() {
    make_f(Bottom);
    swap_x(2, true);
};

template<>
void Cube::make_move<Moves::Dr>() {
    make_fr(Bottom);
    swap_x(2);
};

template<>
void Cube::make_move<Moves::D2>() {
    make_f2(Bottom);
    swap_x(2);
    swap_x(2);
};

template<>
void Cube::make_move<Moves::R>() {
    swap_y_top(2);
    make_f(Right);
}

template<>
void Cube::make_move<Moves::Rr>() {
    swap_y_top(2, true);
    make_fr(Right);
};

template<>
void Cube::make_move<Moves::R2>() {
    swap_y_top(2);
    swap_y_top(2);
    make_f2(Right);
};

template<>
void Cube::make_move<Moves::L>() {
    swap_y_top(0, true);
    make_f(Left);
};

template<>
void Cube::make_move<Moves::Lr>() {
    swap_y_top(0);
    make_fr(Left);
}

template<>
void Cube::make_move<Moves::L2>() {
    make_f2(Left);
    swap_y_top(0);
    swap_y_top(0);
};

template<>
void Cube::make_move<Moves::F>() {
//    transpose(Front);
//    symm_y(Front);
    make_f(Front);
    swap_x_top(2);
}

template<>
void Cube::make_move<Moves::Fr>() {
    make_fr(Front);
    swap_x_top(2, true);
};

template<>
void Cube::make_move<Moves::F2>() {
    make_f2(Front);
    swap_x_top(2);
    swap_x_top(2);
};

template<>
void Cube::make_move<Moves::B>() {
    make_f(Back);
    swap_x_top(0, true);
};

template<>
void Cube::make_move<Moves::Br>() {
//    transpose(Back);
//    symm_x(Back);
    make_fr(Back);
    swap_x_top(0);
}

template<>
void Cube::make_move<Moves::B2>() {
    make_f2(Back);
    swap_x_top(0);
    swap_x_top(0);
};

template<>
void Cube::make_move<Moves::x>() {
    make_f(Right);
    make_fr(Left);
    for (int i = 0; i != 3; ++i) {
        swap_y_top(i);
    }
};

template<>
void Cube::make_move<Moves::xr>() {
    make_fr(Right);
    make_f(Left);
    for (int i = 0; i != 3; ++i) {
        swap_y_top(i, true);
    }
};

template<>
void Cube::make_move<Moves::x2>() {
    make_f2(Right);
    make_f2(Left);
    for (int i = 0; i != 3; ++i) {
        swap_y_top(i);
        swap_y_top(i);
    }
};

template<>
void Cube::make_move<Moves::y>() {
    make_f(Top);
    make_fr(Bottom);
    for (int i = 0; i != 3; ++i) {
        swap_x(i);
    }
};

template<>
void Cube::make_move<Moves::yr>() {
    make_fr(Top);
    make_f(Bottom);
    for (int i = 0; i != 3; ++i) {
        swap_x(i, true);
    }
};

template<>
void Cube::make_move<Moves::y2>() {
    make_f2(Top);
    make_f2(Bottom);
    for (int i = 0; i != 3; ++i) {
        swap_x(i);
        swap_x(i);
    }
};

template<>
void Cube::make_move<Moves::z>() {
    make_f(Front);
    make_fr(Back);
    for (int i = 0; i != 3; ++i) {
        swap_x_top(i);
    }
};

template<>
void Cube::make_move<Moves::zr>() {
    make_fr(Front);
    make_f(Back);
    for (int i = 0; i != 3; ++i) {
        swap_x_top(i, true);
    }
};

template<>
void Cube::make_move<Moves::z2>() {
    make_f2(Front);
    make_f2(Back);
    for (int i = 0; i != 3; ++i) {
        swap_x_top(i);
        swap_x_top(i);
    }
};

template<>
void Cube::make_move<Moves::M>() {
    swap_y_top(1, true);
};

template<>
void Cube::make_move<Moves::M2>() {
    swap_y_top(1);
    swap_y_top(1);
};

template<>
void Cube::make_move<Moves::Mr>() {
    swap_y_top(1);
};

void Cube::initmapping2() {
    mapping2 = {
            {"U",  [this]() { this->make_move<Moves::U>(); }},
            {"U'", [this]() { this->make_move<Moves::Ur>(); }},
            {"U2", [this]() { this->make_move<Moves::U2>(); }},
            {"D",  [this]() { this->make_move<Moves::D>(); }},
            {"D'", [this]() { this->make_move<Moves::Dr>(); }},
            {"D2", [this]() { this->make_move<Moves::D2>(); }},
            {"R",  [this]() { this->make_move<Moves::R>(); }},
            {"R'", [this]() { this->make_move<Moves::Rr>(); }},
            {"R2", [this]() { this->make_move<Moves::R2>(); }},
            {"L",  [this]() { this->make_move<Moves::L>(); }},
            {"L'", [this]() { this->make_move<Moves::Lr>(); }},
            {"L2", [this]() { this->make_move<Moves::L2>(); }},
            {"F",  [this]() { this->make_move<Moves::F>(); }},
            {"F'", [this]() { this->make_move<Moves::Fr>(); }},
            {"F2", [this]() { this->make_move<Moves::F2>(); }},
            {"B",  [this]() { this->make_move<Moves::B>(); }},
            {"B'", [this]() { this->make_move<Moves::Br>(); }},
            {"B2", [this]() { this->make_move<Moves::B2>(); }},
            {"x",  [this]() { this->make_move<Moves::x>(); }},
            {"x'", [this]() { this->make_move<Moves::xr>(); }},
            {"x2", [this]() { this->make_move<Moves::x2>(); }},
            {"y",  [this]() { this->make_move<Moves::y>(); }},
            {"y'", [this]() { this->make_move<Moves::yr>(); }},
            {"y2", [this]() { this->make_move<Moves::y2>(); }},
            {"z",  [this]() { this->make_move<Moves::z>(); }},
            {"z'", [this]() { this->make_move<Moves::zr>(); }},
            {"z2", [this]() { this->make_move<Moves::z2>(); }},
            {"M",  [this]() { this->make_move<Moves::M>(); }},
            {"M'", [this]() { this->make_move<Moves::Mr>(); }},
            {"M2", [this]() { this->make_move<Moves::M2>(); }},
    };
}