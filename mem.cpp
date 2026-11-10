#include "pch.h"
#include "ParameterFactory.h"


namespace
{
	const char* gCurrentPath{};
}

// arguments:
// process name/PID
// virtual address
// size
// read/write/patternscan/getmodulebase
// write data
// pattern
// dump game module
// class informer?
// persistent history?
// --help

// how to return the read data? hexstring? binary file?
// pattern scanning should have an option to only scan pages that are executable
// there should be an option to select operation on game module only or on all modules loaded in the process
// claude should be able to build the vs project




int main(int argc, char* argv[])
{
    auto read = ParameterFactory::create('r');
    read.get()->add_argument("Dbgview.exe");
    read.get()->add_argument("0x00415687");
    read.get()->add_argument("3");
    read.get()->handler();
    
    
    
    
    return 0;
}

