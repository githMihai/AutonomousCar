#if 1

#include "statemachine.h"
#include <unistd.h>

Machine m;

void interruptHandler(sig_t s){
    m.carControl.close();
    exit(1); 
}

int main()
{
    signal (SIGINT, interruptHandler);

    Timer t("machine", 50000, std::bind(&Machine::switchState, &m, (void*)(NULL)));
    t.start();

    // std::thread switchState(&Machine::switchState, &m, (void*)(NULL));

    m.run(NULL);

    // sleep(10000000);
}

#endif