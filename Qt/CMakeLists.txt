set(qtSources
        app.cpp
        appsettings.cpp
        basemainwindow.cpp
        executors.cpp
        fonts.cpp
        loadtsf.cpp
        main.cpp
        mainwindow.cpp
        player.cpp
        selectmicrophonedialog.cpp
        QmlCppBridge.cpp
        qmlpitch.cpp
        qmltimedpitch.cpp
        qmlvxpitch.cpp
        qmlzoomcontroller.cpp
        qml.qrc
        qopenglworkspacewidget.cpp
        qpainterworkspacewidget.cpp
        welcomewindow.cpp
        workspacedrawerwidgetsetup.cpp
        QtUtils/qtutils.cpp
        )

if (APPLE)
    list(APPEND qtSources
            mainwindowosx.mm
            )
endif(APPLE)