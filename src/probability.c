//
// Created by nickl on 05-12-2025.
//

#include "probability.h"

#include <stdlib.h>


int random_chance(int chance)
{
    return rand() % 100 + 1 <= chance;
}
