add_library(ftqlib
        ftqlib.cpp

        # Model implementation files
        BlackSholes.cpp
        LocalVolatility.cpp
        Heston.cpp
)
target_include_directories(ftqlib PUBLIC "../include/")