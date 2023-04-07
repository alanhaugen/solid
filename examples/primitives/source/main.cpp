#include <core/application.h>
#include "primitives.h"

int main(int argc, char **argv)
{
    Application application(argc, argv);

    application.AddScene(new Primitives());

    return application.Exec();
}
