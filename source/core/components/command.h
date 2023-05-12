#ifndef COMMAND_H
#define COMMAND_H

//! https://gameprogrammingpatterns.com/command.html

class Command
{
public:
    Command();

    virtual void Execute() = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;

};

#endif // COMMAND_H
