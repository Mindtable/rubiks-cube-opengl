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

bool Cube::is_correct() {
    const Side cornerLayout[8][3] = {
            Top, Left, Back,
            Top, Back, Right,
            Top, Front, Left,
            Top, Right, Front,

            Bottom, Left, Front,
            Bottom, Front, Right,
            Bottom, Back, Left,
            Bottom, Right, Back
    };

    std::string CubeString = gen_cubestring(false);

    Color color_U = White;
    Color color_D = Yellow;

    std::set<int> countCorners = {320, 301, 315, 352, 402, 410, 451, 425};
    int totalOriCorners = 0;
    std::vector<Color> corner(3, Green);
    for (int i = 0; i != 2; ++i) {

        for (int xk = 0; xk != 2; ++xk) {
            for (int yk = 0; yk != 2; ++yk) {

                corner[0] = faces[cornerLayout[4 * i + 2 * xk + yk][0]][2 * xk][2 * yk];
                corner[1] = faces[cornerLayout[4 * i + 2 * xk + yk][1]][2 * i][2 * i];
                corner[2] = faces[cornerLayout[4 * i + 2 * xk + yk][2]][2 * i][2 - 2 * i];

                for (int ori = 0; ori != 3; ++ori) {
                    if (corner[ori] == color_U || corner[ori] == color_D) {
                        int number = corner[ori] * 100 + corner[(ori + 1) % 3] * 10 + corner[(ori + 2) % 3];
                        if (!countCorners.count(number)) {
                            std::cerr << "Incorrect ori " << number << std::endl;
                            std::cerr << cornerLayout[4 * i + 2 * xk + yk][0] << ' ' << 2 * xk << ' ' << 2 * yk << std::endl;
                            std::cerr << cornerLayout[4 * i + 2 * xk + yk][1] << ' ' << 2 * i << ' ' << 2 * i << std::endl;
                            std::cerr << cornerLayout[4 * i + 2 * xk + yk][2] << ' ' << 2 * i << ' ' << 2 - 2 * i << std::endl;

                            return false;
                        } else {
                            std::cerr << number << ' ' << ori << std::endl;
                            countCorners.erase(number);
                            totalOriCorners += ori;
                        }
                    }
                }

            }
        }
    }

    if (totalOriCorners % 3 != 0) {
        std::cout << "Impossible corner orientation" << std::endl;
        return false;
    }

    const Side EdgeLayout[12][2] = {
            Top, Right,
            Top, Front,
            Top, Left,
            Top, Back,

            Bottom, Right,
            Bottom, Front,
            Bottom, Left,
            Bottom, Back,

            Front, Right,
            Front, Left,
            Back, Left,
            Back, Right
    };

    const int EdgeIndices[12][4]{
            1, 2, 0, 1,
            2, 1, 0, 1,
            1, 0, 0, 1,
            0, 1, 0, 1,

            1, 2, 2, 1,
            0, 1, 2, 1,
            1, 0, 2, 1,
            2, 1, 2, 1,

            1, 2, 1, 0,
            1, 0, 1, 2,
            1, 2, 1, 0,
            1, 0, 1, 2
    };

    std::set<int> CountEdges;
    int TotalOriEdges = 0;

    for (int i = 0; i != 12; ++i) {
        for (auto j: EdgeLayout) {
            if (faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] == static_cast<Color>(j[0]) &&
                faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]] == static_cast<Color>(j[1])) {

                int edge = faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] * 10 +
                           faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]];
                if (!CountEdges.count(edge)) {
                    CountEdges.insert(edge);
                } else {
                    std::cout << "Impossible cube edges count" << std::endl;
                    return false;
                }

                std::cerr << faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] * 10 +
                             faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]] << ' ' << 0 << std::endl;

                break;
            } else if (faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] == static_cast<Color>(j[1]) &&
                       faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]] == static_cast<Color>(j[0])) {

                int edge = faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] * 10 +
                           faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]];

                if (!CountEdges.count(edge)) {
                    CountEdges.insert(edge);
                } else {
                    std::cout << "Impossible cube edges count" << std::endl;
                    return false;
                }
                TotalOriEdges += 1;

                std::cerr << faces[EdgeLayout[i][0]][EdgeIndices[i][0]][EdgeIndices[i][1]] * 10 +
                             faces[EdgeLayout[i][1]][EdgeIndices[i][2]][EdgeIndices[i][3]] << ' ' << 1 << std::endl;
                break;
            }
        }
    }

    if (TotalOriEdges % 2 != 0 || CountEdges.size() != 12) {
        std::cout << "Impossible cube edges layout" << std::endl;
        return false;
    }

    std::cout << "Everything's OK" << std::endl;
    return true;
}