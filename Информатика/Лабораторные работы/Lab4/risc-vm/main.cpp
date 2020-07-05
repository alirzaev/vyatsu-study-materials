#include "includes.hpp"
#include "risc.hpp"

int main(int argc, char** argv)
{
    std::ifstream prog;
    if (argc < 2) {
    	std::cout << "usage: risc-vm program.txt" << std::endl;
    	return 0;
    }
    else {
    	prog.open(argv[1]);
    }
    if (!prog.is_open()) {
    	std::cout << "can't open file" << std::endl;
    	return 0;
    }
    try {
    	risc::load_from_file(prog);
    	risc::run();
    }
    catch (risc::parse_exception &ex) {
    	std::cout << "error while parsing" << std::endl;
    	std::cout << ex.what();
    }
    catch (std::exception &ex) {
    	std::cout << "error while running" << std::endl;
    	std::cout << ex.what();
    }
    return 0;
}
