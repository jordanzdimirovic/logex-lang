#include "loglex-generics.h"
const Keyword keywords[N_KEYWORDS] = {
    {.literal="when", .type=IF},
    {.literal="True", .type=BOOL_TRUE},
    {.literal="true", .type=BOOL_TRUE},
    {.literal="False", .type=BOOL_FALSE},
    {.literal="false", .type=BOOL_FALSE},
    {.literal="set", .type=ASSIGN},
    {.literal="func", .type=FUNC_DEF}
};