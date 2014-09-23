#include "Application.h"


int main(int argc, char **argv)
{
    Application app;
    app.Init();

    while (app.IsRunning())
    {
        app.Update();
    }

    return 0;
}
