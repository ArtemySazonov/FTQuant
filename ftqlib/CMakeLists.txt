add_library(ftqlib
        # Base class implementation files
        MonteCarlo.cpp
        PartialDiffEq.cpp

        # Model implementation files
        BlackSholes.cpp
        LocalVolatility.cpp
)
target_include_directories(ftqlib PUBLIC "../include/")