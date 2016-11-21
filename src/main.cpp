#include <cstdlib>
#include <cstring>
#include <iostream>

#include "main.h"
#include "CApp.h"


static int run_single()
{
    CApp app;
    return app.OnExecute();
}


int main(int argc, char* argv[])
{
        return run_single();
}
