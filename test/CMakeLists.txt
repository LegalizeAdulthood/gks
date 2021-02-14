find_package(Catch2 REQUIRED)

add_executable(test
    main.cpp
    test.cpp
    error.h
    error.cpp
    gkcl.cpp)
target_link_libraries(test PUBLIC gks Catch2::Catch2)

add_custom_command(TARGET test POST_BUILD COMMAND test)
