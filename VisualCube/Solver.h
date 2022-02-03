//
// Created by MTBLE on 18.01.2022.
//

#include "dependences.h"
#include "Cube.h"
#include <random>

#ifndef GEN_ALGO_SOLVER_H
#define GEN_ALGO_SOLVER_H


class Solver {
private:
    std::random_device m_RD;
    std::mt19937 m_RNG;

public:
    Solver() : m_RNG(m_RD()) {
        NumberOfCycles = 0;
        HowManyWorlds = 0;
        Elitaria = 0;
        NumberOfCubes = 0;

        srand(time(nullptr));
    }

    Solver(int population_size, int max_generations, int max_resets, int elitism_num)
            : m_RNG(m_RD()), NumberOfCubes(population_size), NumberOfCycles(max_generations), HowManyWorlds(max_resets),
              Elitaria(elitism_num), m_ClassicMoveRandom(0, 17),
              m_ElitariaRandom(0, Elitaria - 1),
              m_EvolutionRandom(0, 5),
              m_PermutationPoolRandom(0, 14),
              m_RotationAxisRandom(0, 5),
              m_RotationZRandom(0, 2) {
        ;
    }

    std::string solve(const Cube &scrambledCube) {
        if (scrambledCube.get_fitness() == 0) {
            std::cout << "Naughty boy! Cube's already been solved" << std::endl;
            return "";
        }

        std::cout << "Started solving..." << std::endl;

        for (int r = 0; r != HowManyWorlds; ++r) {
            std::vector<Cube> cubes(NumberOfCubes);
            for (auto &cube: cubes) {
                cube = scrambledCube;
                cube.exec_perm(ClassicMoves[m_ClassicMoveRandom(m_RNG)]);
                cube.exec_perm(ClassicMoves[m_ClassicMoveRandom(m_RNG)]);
            }

            for (int g = 0; g != NumberOfCycles; ++g) {
                std::sort(cubes.begin(), cubes.end(),
                          [](const Cube &a, const Cube &b) -> bool { return a.get_fitness() < b.get_fitness(); });
                if (g % 30 == 0) {
                    std::cout << g + 1 << " generation " << cubes[0].get_fitness() << " current fitness" << std::endl;
                }
                for (int i = 0; i != NumberOfCubes; ++i) {
                    if (cubes[i].get_fitness() == 0) {
                        std::string solution;
                        for (const auto &elem: cubes[i].GetRoute()) {
                            solution += elem + " ";
                        }
                        return solution;
                    }

                    if (i > Elitaria) {
                        cubes[i] = cubes[m_ElitariaRandom(m_RNG)];

                        int evo_type = m_EvolutionRandom(m_RNG);
                        if (evo_type == 0) {
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        } else if (evo_type == 1) {
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        } else if (evo_type == 2) {
                            cubes[i].exec_perm(RotationAxis[m_RotationAxisRandom(m_RNG)]);
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        } else if (evo_type == 3) {
                            cubes[i].exec_perm(RotationZ[m_RotationZRandom(m_RNG)]);
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        } else if (evo_type == 4) {
                            cubes[i].exec_perm(RotationAxis[m_RotationAxisRandom(m_RNG)]);
                            cubes[i].exec_perm(RotationZ[m_RotationZRandom(m_RNG)]);
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        } else if (evo_type == 5) {
                            cubes[i].exec_perm(RotationZ[m_RotationZRandom(m_RNG)]);
                            cubes[i].exec_perm(RotationAxis[m_RotationAxisRandom(m_RNG)]);
                            cubes[i].exec_perm(PermutationPool.at(m_PermutationPoolRandom(m_RNG)));
                        }
                    }
                }
            }
        }

        return "";
    }

private:
    int NumberOfCubes, NumberOfCycles, HowManyWorlds, Elitaria;

    std::uniform_int_distribution<int> m_ClassicMoveRandom;
    std::uniform_int_distribution<int> m_ElitariaRandom;
    std::uniform_int_distribution<int> m_EvolutionRandom;
    std::uniform_int_distribution<int> m_PermutationPoolRandom;
    std::uniform_int_distribution<int> m_RotationAxisRandom;
    std::uniform_int_distribution<int> m_RotationZRandom;

    const std::vector<std::string> ClassicMoves = {"U", "U'", "U2", "D", "D'", "D2", "R", "R'", "R2", "L", "L'",
                                                   "L2", "F", "F'", "F2", "B", "B'", "B2"};
    const std::vector<std::string> RotationAxis = {"x", "x'", "x2", "y", "y'", "y2"};

    const std::vector<std::string> RotationZ = {"z", "z'", "z2"};
    const std::vector<std::string> PermutationPool =
            {
                    "F' L' B' R' U' R U' B L F R U R' U",
                    "F R B L U L' U B' R' F' L' U' L U'",
                    "U2 B U2 B' R2 F R' F' U2 F' U2 F R'",
                    "U2 R U2 R' F2 L F' L' U2 L' U2 L F'",
                    "U' B2 D2 L' F2 D2 B2 R' U'",
                    "U B2 D2 R F2 D2 B2 L U",
                    "D' R' D R2 U' R B2 L U' L' B2 U R2",
                    "D L D' L2 U L' B2 R' U R B2 U' L2",
                    "R' U L' U2 R U' L R' U L' U2 R U' L U'",
                    "L U' R U2 L' U R' L U' R U2 L' U R' U",
                    "F' U B U' F U B' U'",
                    "F U' B' U F' U' B U",
                    "L' U2 L R' F2 R",
                    "R' U2 R L' B2 L",
                    "M2 U M2 U2 M2 U M2",
            };
};


#endif //GEN_ALGO_SOLVER_H
