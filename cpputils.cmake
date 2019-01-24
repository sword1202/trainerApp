set(cppUtilsSources
        CppUtils/AudioUtils.cpp
        CppUtils/AudioUtils.h
        CppUtils/WAVFile.cpp
        CppUtils/WAVFile.h
        CppUtils/TimeUtils.cpp
        CppUtils/TimeUtils.h
        CppUtils/OperationCanceler.cpp
        CppUtils/OperationCanceler.h
        CppUtils/Random.cpp
        CppUtils/Bitmap.cpp
        CppUtils/Color.cpp
        CppUtils/BinaryMath.h
        )

set(cppUtilsTests
        CppUtils/tests/GeometryUtilsTest.cpp
        CppUtils/tests/BinaryMathTests.cpp
        CppUtils/tests/ColorTests.cpp
        CppUtils/tests/StringsTest.cpp
        )

list(TRANSFORM cppUtilsSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
list(TRANSFORM cppUtilsTests PREPEND ${CMAKE_CURRENT_LIST_DIR}/)