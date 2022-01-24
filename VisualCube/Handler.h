//
// Created by MTBLE on 18.01.2022.
//

#include "dependences.h"
#include "Solver.h"
#include "Cube.h"

#ifndef GEN_ALGO_HANDLER_H
#define GEN_ALGO_HANDLER_H


class Handler {
public:
    Handler() = default;

    void start() {
        std::cout << "Hello traveller!\n"
                     "There is a list of available options\n"
                     "1. Import cube state from file\n"
                     "2. Output cube state in console\n"
                     "3. Output cube state in file\n"
                     "4. Solve permutation from file" << std::endl;
        std::string input;
        Cube test;
        while (std::cin >> input) {
            if (input == "1") {
                std::cout << "Enter filename (must be in the same dir): " << std::flush;
                std::cin >> input;
                test.fromfile(input.c_str());
            } else if (input == "2") {
                std::cout << test.gen_cubestring(true);
            } else if (input == "3") {
                std::cout << "Enter filename (must be in the same dir): " << std::flush;
                std::cin >> input;

                std::ofstream fout(input);
                fout << test.gen_cubestring(true);
            } else if (input == "4") {
                std::cout << "Enter filename (must be in the same dir): " << std::flush;
                std::cin >> input;
                std::cout << input;
                Solver solver(500, 300, 10, 50);
                std::ifstream fin(input);
                std::getline(fin, input);
                std::cout << "started solving.." << std::endl;
                solver.solve(input);
            }

            std::cout << "Press any button..." << std::endl;
            system("PAUSE");
            system("CLS");
            std::cout << "Hello traveller!\n"
                         "There is a list of available options\n"
                         "1. Import cube state from file\n"
                         "2. Output cube state in console\n"
                         "3. Output cube state in file\n"
                         "4. Solve permutation from file" << std::endl;
        }
    }
};


#endif //GEN_ALGO_HANDLER_H
