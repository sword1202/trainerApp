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
        WelcomeWindow.cpp
        Workspace/WorkspaceDrawerWidgetSetup.cpp
        QtUtils/qtutils.cpp
        Workspace/TextImagesFactory.cpp
        )

if (APPLE)
    list(APPEND qtSources
            QtUtils/qtutils.mm
            MainWindowOsx.mm
            Workspace/Metal/QMetalWidget.mm
            Workspace/Metal/MetalWorkspaceWidget.mm
            Workspace/Metal/MetalView.mm
            ../MetalTest/MetalTest/MetalView2.mm
            )
else()
    list(APPEND qtSources
            Workspace/OpenglWorkspaceWidget.cpp
            )
endif(APPLE)