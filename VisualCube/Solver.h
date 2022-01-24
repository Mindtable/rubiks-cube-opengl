//
// Created by MTBLE on 18.01.2022.
//

#include "dependences.h"
#include "Cube.h"

#ifndef GEN_ALGO_SOLVER_H
#define GEN_ALGO_SOLVER_H


class Solver {
public:
    Solver() {
        NumberOfCycles = 0;
        HowManyWorlds = 0;
        Elitaria = 0;
        NumberOfCubes = 0;
        srand(time(nullptr));
    }

    Solver(int population_size, int max_generations, int max_resets, int elitism_num)
            : NumberOfCubes(population_size), NumberOfCycles(max_generations), HowManyWorlds(max_resets),
              Elitaria(elitism_num) {
        srand(time(nullptr));;
    }

    void solve(const std::string &scramble) {
        for (int r = 0; r != HowManyWorlds; ++r) {
//            std::cout << "try " << r +1 << std::endl;


            std::vector<Cube> cubes(NumberOfCubes);
            for (auto &cube: cubes) {
                cube.exec_perm(scramble);
                //FIXME Probably should use not c-rand function
                cube.exec_perm(ClassicMoves[rand() % 18]);
                cube.exec_perm(ClassicMoves[rand() % 18]);
            }

            for (int g = 0; g != NumberOfCycles; ++g) {
//                std::cout << "gen " << g +1 << std::endl;



                std::sort(cubes.begin(), cubes.end(),
                          [](const Cube &a, const Cube &b) -> bool { return a.get_fitness() < b.get_fitness(); });

                if (g % 30 == 0) {
                    std::cout << g + 1  << " " <<  cubes[0].get_fitness() << std::endl;
                }
//                std::cout << "qq";

                for (int i = 0; i != NumberOfCubes; ++i) {
//                    std::cout << "ent " << i +1 << std::endl;

                    if (cubes[i].get_fitness() == 0) {
                        //FIXME
//                        cubes[i].print();
                        std::cout << "Solution (including start permutation)\n";
                        for (const auto &elem: cubes[i].route) {
                            std::cout << elem << " ";
                        }
                        std::cout << std::endl;
                        return;
                    }

                    if (i > Elitaria) {
                        cubes[i] = cubes[rand() % Elitaria];

                        int evo_type = rand() % 6;


//                        std::cout << "evo " << evo_type << std::endl;


                        if (evo_type == 0) {
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        } else if (evo_type == 1) {
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        } else if (evo_type == 2) {
                            cubes[i].exec_perm(RotationAxis[rand() % RotationAxis.size()]);
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        } else if (evo_type == 3) {
                            cubes[i].exec_perm(RotationZ[rand() % RotationZ.size()]);
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        } else if (evo_type == 4) {
                            cubes[i].exec_perm(RotationAxis[rand() % RotationAxis.size()]);
                            cubes[i].exec_perm(RotationZ[rand() % RotationZ.size()]);
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        } else if (evo_type == 5) {
                            cubes[i].exec_perm(RotationZ[rand() % RotationZ.size()]);
                            cubes[i].exec_perm(RotationAxis[rand() % RotationAxis.size()]);
                            cubes[i].exec_perm(PermutationPool.at(rand() % PermutationPool.size()));
                        }
                    }
                }


            }


        }
    }

private:
    int NumberOfCubes, NumberOfCycles, HowManyWorlds, Elitaria;
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
