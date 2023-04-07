#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "core/containers/linkedlist.h"
#include "core/containers/hash.h"

template <class Datatype>
class Arguments {
public:
    Arguments(int numberOfArguments, char **argumentArray)
        : arguments_(DEFAULT_HASH_SIZE, Hash<String, LinkedList<Datatype> >::StringHash) {
        (void)numberOfArguments;
        (void)argumentArray;
    }

    LinkedList<Datatype> *Find(Datatype option) {
        if (arguments_.Find(option)) {
            return &arguments_.Find(option)->data_;
        }

        return NULL;
    }

    template <typename argumentType>
    argumentType *GetArgument(String option) {
        /*String *argument = Find(option);

        if (argument == NULL) {
            return NULL;
        }
        //else if (argument[0] == "=") {
        //}

        argumentType returnArgument = argumentType(argument);

        return returnArgument;*/
        return NULL;
    }

    template <typename argumentType>
    void SetArgument(String option, argumentType &variableToSet) {
        argumentType *argument = GetArgument<argumentType>(option);

        if (argument != NULL) {
            variableToSet = *argument;
        }
    }

    inline LinkedList<Datatype> *operator[](String key) { return Find(key); }

private:
    Hash<String, LinkedList<Datatype> > arguments_;
};

#endif // ARGUMENTS_H
