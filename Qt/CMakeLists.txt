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
        QmlWorkspaceZoomController.cpp
        qml.qrc
        Workspace/OpenglWorkspaceWidget.cpp
        WelcomeWindow.cpp
        Workspace/WorkspaceDrawerWidgetSetup.cpp
        QtUtils/qtutils.cpp
        Workspace/TextImagesFactory.cpp
        )

if (APPLE)
    list(APPEND qtSources
            MainWindowOsx.mm
            Workspace/Metal/QMetalWidget.mm
            Workspace/Metal/MetalWorkspaceWidget.mm
            )
endif(APPLE)