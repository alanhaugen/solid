#include <core/application.h>
#include "example1.h"

int main(int argc, char **argv)
{
    Application application(argc, argv);

    application.AddScene(new Example1());

    return application.Exec();
}
