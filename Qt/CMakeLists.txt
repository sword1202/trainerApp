set(qtSources
        VxApp.cpp
        AppSettings.cpp
        BaseMainWindow.cpp
        Executors.cpp
        Fonts.cpp
        icons.qrc
        LoadTsf.cpp
        main.cpp
        ProjectWindow.cpp
        QtBridge/QtMvxPlayer.cpp
        SelectMicrophoneDialog.cpp
        QtBridge/QtCppBridge.cpp
        QtBridge/QtPitch.cpp
        QtBridge/QtWorkspaceZoomController.cpp
        qml.qrc
        Welcome/ProjectItem.cpp
        Welcome/WelcomeWindow.cpp
        Welcome/WelcomeWindow.ui
        Workspace/WorkspaceDrawerWidgetSetup.cpp
        Utils/QtUtils.cpp
        Workspace/TextImagesFactory.cpp
        )

if (APPLE)
    list(APPEND qtSources
            Utils/QtUtils.mm
            BaseMainWindowMacOS.mm
            Workspace/Metal/QMetalWidget.mm
            Workspace/Metal/MetalWorkspaceWidget.mm
            Workspace/Metal/MetalView.mm
            )
else()
    list(APPEND qtSources
            Workspace/OpenglWorkspaceWidget.cpp
            )
endif(APPLE)
