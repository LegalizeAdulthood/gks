#include "error.h"

std::vector<Error> g_recordedErrors;

Gint gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    g_recordedErrors.push_back({static_cast<GError>(errNum), static_cast<GFunction>(funcName)});
    return GERROR_NONE;
}
