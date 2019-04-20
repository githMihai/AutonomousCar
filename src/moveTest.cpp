#if 1

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <unistd.h>

#include <signal.h>

#include "carcontrol.h"

#include "findfile.h"

void* print(void* str)
{
    std::cout << (char*)str << std::endl;
}

CarControl *c;

void interruptHandler(sig_t s){
    c->close();
    exit(1); 
}


int main(int argc, char **argv)
{
    signal (SIGINT, interruptHandler);

    c = new CarControl();

    c->brake(10.0);

    c->close();
    
    return 0;
}


#endif