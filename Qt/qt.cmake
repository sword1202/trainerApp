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
        Project/ProjectWindow.h
        Project/SelectMicrophoneDialog.cpp
        Project/SelectMicrophoneDialog.h
        Project/SingingResultDialog.cpp
        Project/SingingResultDialog.h
        QtBridge/QtMvxPlayer.cpp
        QtBridge/QtCppBridge.cpp
        QtBridge/QtPitch.cpp
        QtBridge/QtWorkspaceZoomController.cpp
        qml.qrc
        Welcome/WelcomeWindow.cpp
        Workspace/WorkspaceDrawerWidgetSetup.cpp
        Utils/QtUtils.cpp
        Workspace/QtDrawerTextImagesFactory.cpp
        Utils/BaseQmlWidget.cpp
        Utils/BaseQmlWidget.h
        App/VxAppUtils.cpp
        App/VxAppUtils.h
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

list(TRANSFORM qtSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
