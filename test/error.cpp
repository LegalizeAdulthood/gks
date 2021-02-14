#include "error.h"

std::vector<Error> g_recordedErrors;

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    g_recordedErrors.push_back({errNum, funcName});
}
