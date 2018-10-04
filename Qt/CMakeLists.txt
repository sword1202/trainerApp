set(qtSources
        VxApp.cpp
        AppSettings.cpp
        BaseMainWindow.cpp
        Executors.cpp
        Fonts.cpp
        LoadTsf.cpp
        main.cpp
        MainWindow.cpp
        Player.cpp
        SelectMicrophoneDialog.cpp
        QmlCppBridge.cpp
        QmlPitch.cpp
        QmlTimedPitch.cpp
        QmlVxPitch.cpp
        QmlZoomController.cpp
        qml.qrc
        QOpenglWorkspaceWidget.cpp
        QPainterWorkspaceWidget.cpp
        WelcomeWindow.cpp
        WorkspaceDrawerWidgetSetup.cpp
        QtUtils/qtutils.cpp
        )

if (APPLE)
    list(APPEND qtSources
            MainWindowOsx.mm
            )
endif(APPLE)