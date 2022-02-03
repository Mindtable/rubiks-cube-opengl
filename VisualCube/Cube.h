//
// Created by MTBLE on 17.01.2022.
//

#include "dependences.h"
#include <set>
#include <algorithm>
#include <random>

#ifndef GEN_ALGO_CUBE_H
#define GEN_ALGO_CUBE_H


class Cube {
private:
    using side_row = std::vector<Color>;
    using side_matrix = std::vector<side_row>;

    std::map<std::string, std::function<void()>> mapping2;
    int fitness;
    std::list<std::string> route;

public:
    Cube() {
        for (int i = Front; i != Back + 1; ++i) {
            faces[static_cast<Side>(i)] = side_matrix(3, side_row(3, static_cast<Color>(i)));
        }
        initmapping2();
        fitness = 0;
    }

    explicit Cube(const std::string &input) : Cube() {
        std::map<char, Color> mapping = {
                {'G', Green},
                {'O', Orange},
                {'R', Red},
                {'W', White},
                {'Y', Yellow},
                {'B', Blue}
        };
        auto it = input.begin();
        for (int i = Front; i != Back + 1; ++i) {
            for (int j = 0; j != 9; ++j) {
                faces[static_cast<Side>(i)][j / 3][j % 3] = mapping[*it];
                ++it;
            }
        }

        refresh_fitness();
    }

    Cube &operator=(const Cube &other) {
        //FIXME
        faces = other.faces;
        fitness = other.fitness;
        route = other.route;
        return *this;
    }

    void initmapping2();

    [[nodiscard]] int get_fitness() const {
        return fitness;
    }

    void exec_move(const std::string &input) {
        try {
            mapping2.at(input)();
            route.push_back(input);
            refresh_fitness();
        } catch (std::exception &ex) {
            std::cerr << "FAIL 1" << std::endl;
            std::cerr << input << " " << input.size() << std::endl;
            throw ex;
        }
    }

    void exec_perm(const std::string &input) {
        std::istringstream parcer(input);
        std::string move;
        while (parcer >> move) {
            try {
                mapping2.at(move)();
                route.push_back(move);
            } catch (const std::exception &ex) {
                std::cerr << "Failed " << move << std::endl;
                throw std::out_of_range(move);
            }
        }

        refresh_fitness();
    }

    void ClearRoute() {
        route.clear();
    }

    std::list<std::string> &GetRoute() {
        return route;
    }

    void fromfile(const char *filename) {
        std::ifstream fin(filename);

        if (!fin.is_open()) {
            throw std::runtime_error("File not found");
        }

        std::string input;
        fin >> input;
        fin.close();
        if (input.size() == 1 || input.size() == 2) {
            perm_fromfile(filename);
        } else if (input.size() == 3 || input.size() == 9 || input.size() == 54) {
            cubestring_fromfile(filename, input.size());
        } else {
            throw std::runtime_error("Incorrect file format");
        }
    }

    std::string gen_cubestring(bool format = false) {
        if (format) {
            return gen_formatted_string();
        }
        const std::string mapping = "GORWYB";
        std::string result;
        for (int i = Front; i != Back + 1; ++i) {
            for (int j = 0; j != 3; ++j) {
                for (int k = 0; k != 3; ++k) {
                    result += mapping[faces[static_cast<Side>(i)][j][k]];
                }
            }
        }
        return result;
    }

    void tofile(const char *filename, bool format = false) {
        std::ofstream fout(filename);
        fout << gen_cubestring(format);
    }

    bool is_correct();

    std::string shuffle();
public:
    std::map<Side, side_matrix> faces;

private:
    std::string gen_formatted_string(){
        std::string cubestring;
        std::string mapping = "GORWYB";

        for (int i = 0; i != 3; ++i) {
            cubestring += "      ";
            for (int j = 0; j != 3; ++j) {
                cubestring += mapping[faces[Top][i][j]];
                cubestring += " ";
            }
            cubestring += "\n";
        }

        for (int i = 0; i != 3; ++i) {
            for (int j = 0; j != 12; ++j) {
                if (j < 3) {
                    cubestring += mapping[faces[Left][i][j]];
                    cubestring += " ";
                } else if (j < 6) {
                    cubestring += mapping[faces[Front][i][j - 3]];
                    cubestring += " ";
                } else if (j < 9) {
                    cubestring += mapping[faces[Right][i][j - 6]];
                    cubestring += " ";
                } else {
                    cubestring += mapping[faces[Back][i][j - 9]];
                    cubestring += " ";
                }
            }
            cubestring += "\n";
        }
        for (int i = 0; i != 3; ++i) {
            cubestring += "      ";
            for (int j = 0; j != 3; ++j) {
                cubestring += mapping[faces[Bottom][i][j]];
                cubestring += " ";
            }
            cubestring += "\n";
        }
        return cubestring;
    }

    void perm_fromfile(const char *filename) {
        std::ifstream fin(filename);
        std::string input;
        while (fin >> input) {
            exec_move(input);
        }
    }

    void cubestring_fromfile(const char *filename, int comp_size = 9) {
        std::ifstream fin(filename);
        std::string cubestring;

        if (comp_size == 54) {
            fin >> cubestring;
            Cube temp(cubestring);
            std::swap(*this, temp);
            return;
        }

        std::map<char, std::string> mapping;

        for (int i = 0; i != 6; ++i) {
            std::string input;
            fin >> input;
            if (comp_size == 3) {
                std::string temp_input;
                fin >> temp_input;
                input += temp_input;
                fin >> temp_input;
                input += temp_input;
            }
            mapping.emplace(input[4], input);
        }

        cubestring = mapping.at('G') + mapping.at('O') + mapping.at('R') + mapping.at('W') + mapping.at('Y') +
                     mapping.at('B');
        Cube temp(cubestring);
        std::swap(*this, temp);
    }

    void refresh_fitness() {
        fitness = 0;
        for (const auto &pair: faces) {
            const auto &face = pair.second;
            Color center = face[1][1];
            for (const auto &row: face) {
                for (const auto &cubie: row) {
                    if (cubie != center) {
                        ++fitness;
                    }
                }
            }
        }
    }

    void transpose(Side side) {
        auto &values = faces[side];

        for (int n = 0; n != 2; ++n) {
            for (int m = n + 1; m != 3; ++m) {
                std::swap(values[n][m], values[m][n]);
            }
        }
    }

    void symm_x(Side side) {
        auto &values = faces[side];
        for (int n = 0; n != 3; ++n) {
            std::swap(values[0][n], values[2][n]);
        }
    }

    void symm_y(Side side) {
        auto &values = faces[side];
        for (int n = 0; n != 3; ++n) {
            std::swap(values[n][0], values[n][2]);
        }
    }

    void swap_x_top(int number, bool reverse = false) {
        Side top = Top, right = Right, bottom = Bottom, left = Left;
        if (reverse) {
            std::swap(right, left);
        }

        // FIXME Maybe some optimization needed
        make_f(Left);
        make_fr(Right);
        make_f2(Bottom);

        std::swap(faces[top][number], faces[left][number]);
        std::swap(faces[left][number], faces[bottom][number]);
        std::swap(faces[bottom][number], faces[right][number]);

        make_fr(Left);
        make_f(Right);
        make_f2(Bottom);
    }

    void swap_x(int number, bool reverse = false) {
        Side front = Front, right = Right, back = Back, left = Left;
        if (reverse) {
            std::swap(right, left);
        }

        std::swap(faces[front][number], faces[right][number]);
        std::swap(faces[right][number], faces[back][number]);
        std::swap(faces[back][number], faces[left][number]);
    }

    void swap_y_top(int number, bool reverse = false) {
        Side top = Top, front = Front, bottom = Bottom, back = Back;
        if (reverse) {
            std::swap(front, back);
        }
        make_f(Top);
        make_f(Front);
        make_fr(Back);
        make_f(Bottom);

        std::swap(faces[top][number], faces[front][number]);
        std::swap(faces[front][number], faces[bottom][number]);
        std::swap(faces[bottom][number], faces[back][number]);

        make_fr(Top);
        make_fr(Front);
        make_f(Back);
        make_fr(Bottom);
    }

    void make_f(Side side) {
        transpose(side);
        symm_y(side);
    }

    void make_fr(Side side) {
        transpose(side);
        symm_x(side);
    }

    void make_f2(Side side) {
        symm_x(side);
        symm_y(side);
    }

protected:
    template<Moves::Move T>
    void make_move() {
        ;
    }
};

#endif //GEN_ALGO_CUBE_H
