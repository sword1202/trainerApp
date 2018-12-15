set(qtSources
        App/VxApp.cpp
        App/AppSettings.cpp
        Utils/BaseMainWindow.cpp
        App/Executors.cpp
        App/Fonts.cpp
        icons.qrc
        App/LoadTsf.cpp
        main.cpp
        Project/ProjectWindow.cpp
        QtBridge/QtMvxPlayer.cpp
        Project/SelectMicrophoneDialog.cpp
        QtBridge/QtCppBridge.cpp
        QtBridge/QtPitch.cpp
        QtBridge/QtWorkspaceZoomController.cpp
        qml.qrc
        Welcome/ProjectItemWidget.cpp
        Welcome/WelcomeWindow.cpp
        Welcome/WelcomeWindow.ui
        Workspace/WorkspaceDrawerWidgetSetup.cpp
        Utils/QtUtils.cpp
        Workspace/TextImagesFactory.cpp
        Utils/BaseQmlWidget.cpp
        Utils/BaseQmlWidget.h
        )

if (APPLE)
    list(APPEND qtSources
            Utils/QtUtils.mm
            Utils/BaseMainWindowMacOS.mm
            Workspace/Metal/QMetalWidget.mm
            Workspace/Metal/MetalWorkspaceWidget.mm
            Workspace/Metal/MetalView.mm
            )
else()
    list(APPEND qtSources
            Workspace/OpenglWorkspaceWidget.cpp
            )
endif(APPLE)
