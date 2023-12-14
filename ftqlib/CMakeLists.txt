add_library(ftqlib
        # Base class implementation files
        SyntaxParser.cpp
        MonteCarlo.cpp
        PartialDiffEq.cpp

        # Model implementation files
        BlackSholes.cpp
        LocalVolatility.cpp

        RectBivariateSpline.cpp
)
target_include_directories(ftqlib PUBLIC "../include/")