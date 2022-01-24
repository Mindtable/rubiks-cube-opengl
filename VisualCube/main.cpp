#include "Cube.h"
#include "Solver.h"
#include "Handler.h"

int main() {
    Handler work;
    work.start();
    return 0;

    double start = clock();
    std::string cubestring = "GGGGGGGGGOOOOOOOOORRRRRRRRRWWWWWWWWWYYYYYYYYYBBBBBBBBB";
    std::ifstream fin("kek.txt");
    std::string input;
    std::cout << fin.is_open();

    Cube test;
    test.fromfile("intput.txt");
    test.tofile("output.txt", true);
//    test.fromfile("output.txt");
    std::cout << test.gen_cubestring(true);
    return 0;
    std::cout << std::endl;
    std::cout << test.get_fitness() << std::endl;
    Solver test_solve(500, 800, 10, 50);
    test_solve.solve("U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2");
    test.exec_perm("U R2 F B R B2 R U2 L B2 R U' D' R2 F R' L B2 U2 F2");
//    test.print();


    double end = clock();
    std::cout << std::setprecision(10) << (end - start) / CLOCKS_PER_SEC << std::endl;
}
