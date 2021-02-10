add_library(gks
    include/gks/gks.h
    gks.c
    ../doc/Level_ma.txt)
target_include_directories(gks PUBLIC include)
