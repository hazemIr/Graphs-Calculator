#include "final.h"
#include "shell.h"

using std::endl;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

int main(int argc, char **argv) {
    if (is_shell_flag(argc)) {
        Shell shell;
        shell.run();
    }
    else if (is_batch_flag(argc)) {
        std::ifstream inputfile;
        inputfile.open(argv[1]);
        std::ofstream outputfile;
        outputfile.open(argv[2]);
        Shell shell(false, inputfile, outputfile);
        shell.run();
        inputfile.close();
        outputfile.close();
    }
    else {
        std::cerr << "Error: argc doesn't meet conditions (1 for shell, 3 for batch): " << argc << endl;
        return 0;
    }
}

static bool is_shell_flag(int argc) {
    return argc == 1;
}

static bool is_batch_flag(int argc) {
    return argc == 3;
}