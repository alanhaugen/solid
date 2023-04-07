#include <core/application.h>
#include "minigames.h"

int main(int argc, char **argv)
{
    Application application(argc, argv);

    application.AddScene(new MiniGames());

    return application.Exec();
}
