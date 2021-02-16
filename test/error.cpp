#include "error.h"

std::vector<Error> g_recordedErrors;

void gerrorhand(Gint errNum, Gint funcName, Gfile *errFile)
{
    g_recordedErrors.push_back({static_cast<GError>(errNum), static_cast<GFunction>(funcName)});
}
