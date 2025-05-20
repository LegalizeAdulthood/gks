add_library(gks
    include/gks/gks.h
    include/gks/gkserror.h
    gks.cpp
    ../doc/Level_ma.txt)
target_include_directories(gks PUBLIC include)
target_folder(gks "Libraries")

add_library(gks-error gkserror.cpp)
target_link_libraries(gks-error PUBLIC gks)
target_folder(gks-error "Libraries")
