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
        )

list(TRANSFORM cppUtilsSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)