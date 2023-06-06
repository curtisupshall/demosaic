#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif