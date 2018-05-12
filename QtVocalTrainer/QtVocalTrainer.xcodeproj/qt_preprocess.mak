#############################################################################
# Makefile for building: QtVocalTrainer.app/Contents/MacOS/QtVocalTrainer
# Generated by qmake (3.1) (Qt 5.11.0)
# Project:  QtVocalTrainer.pro
# Template: app
# Command: /Users/semyon/Qt/5.11.0/clang_64/bin/qmake -o QtVocalTrainer.xcodeproj/project.pbxproj QtVocalTrainer.pro -spec macx-xcode
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /Users/semyon/Qt/5.11.0/clang_64/bin/moc
UIC       = 
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_QUICK_LIB -DQT_GUI_LIB -DQT_QML_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB
INCPATH       = -I. -I../../../Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers -I../../../Qt/5.11.0/clang_64/lib/QtGui.framework/Headers -I../../../Qt/5.11.0/clang_64/lib/QtQml.framework/Headers -I../../../Qt/5.11.0/clang_64/lib/QtNetwork.framework/Headers -I../../../Qt/5.11.0/clang_64/lib/QtCore.framework/Headers -I. -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks/OpenGL.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks/AGL.framework/Headers -I../../../Qt/5.11.0/clang_64/mkspecs/macx-clang -F/Users/semyon/Qt/5.11.0/clang_64/lib
DEL_FILE  = rm -f
MOVE      = mv -f

preprocess: compilers
clean preprocess_clean: compiler_clean

mocclean: compiler_moc_header_clean compiler_moc_objc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_objc_header_make_all compiler_moc_source_make_all

check: first

benchmark: first

compilers: qrc_qml.cpp moc_predefs.h moc_MainWindow.cpp moc_QmlCppBridge.cpp moc_qmlopenglitem.cpp
compiler_rcc_make_all: qrc_qml.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_qml.cpp
qrc_qml.cpp: qml.qrc \
		../../../Qt/5.11.0/clang_64/bin/rcc \
		qml/Workspace.qml \
		qml/Tempo.qml \
		qml/ButtonShadow.qml \
		qml/ToggleButton.qml \
		qml/FeaturesToggleButton.qml \
		qml/Player.qml \
		qml/SvgImage.qml \
		qml/ImageButton.qml \
		qml/main.qml \
		qml/images/to_the_begining_hover.svg \
		qml/images/metronome_off.svg \
		qml/images/lyrics_show_button_on.svg \
		qml/images/to_the_begining.svg \
		qml/images/player_background.svg \
		qml/images/loop_off.svg \
		qml/images/lyrics_show_button_off.png \
		qml/images/loop_on.svg \
		qml/images/go_forward_hover.svg \
		qml/images/metronome_on.svg \
		qml/images/play_on.svg \
		qml/images/metronome_off.png \
		qml/images/player_background.png \
		qml/images/play_off.svg \
		qml/images/tracks_show_button_off.svg \
		qml/images/go_forward.svg \
		qml/images/go_back_hover.svg \
		qml/images/tracks_show_button_on.svg \
		qml/images/tempo_background.svg \
		qml/images/go_back.svg \
		qml/images/lyrics_show_button_off.svg \
		qml/images/tracks_show_button_off.png \
		qml/fonts/Lato-Regular.ttf \
		qml/fonts/Lato-Bold.ttf
	/Users/semyon/Qt/5.11.0/clang_64/bin/rcc -name qml qml.qrc -o qrc_qml.cpp

compiler_moc_predefs_make_all: moc_predefs.h
compiler_moc_predefs_clean:
	-$(DEL_FILE) moc_predefs.h
moc_predefs.h: ../../../Qt/5.11.0/clang_64/mkspecs/features/data/dummy.cpp
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -pipe -stdlib=libc++ -std=c++17 -O2 -std=gnu++11 -Wall -W -dM -E -o moc_predefs.h ../../../Qt/5.11.0/clang_64/mkspecs/features/data/dummy.cpp

compiler_moc_header_make_all: moc_MainWindow.cpp moc_QmlCppBridge.cpp moc_qmlopenglitem.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_MainWindow.cpp moc_QmlCppBridge.cpp moc_qmlopenglitem.cpp
moc_MainWindow.cpp: ../../../Qt/5.11.0/clang_64/lib/QtCore.framework/Headers/QObject \
		../../../Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers/QQuickView \
		MainWindow.h \
		moc_predefs.h \
		../../../Qt/5.11.0/clang_64/bin/moc
	/Users/semyon/Qt/5.11.0/clang_64/bin/moc $(DEFINES) --include ./moc_predefs.h -I/Users/semyon/Qt/5.11.0/clang_64/mkspecs/macx-clang -I/Users/semyon/Projects/VocalTrainer/QtVocalTrainer -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtGui.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQml.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtNetwork.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/usr/include -F/Users/semyon/Qt/5.11.0/clang_64/lib MainWindow.h -o moc_MainWindow.cpp

moc_QmlCppBridge.cpp: ../../../Qt/5.11.0/clang_64/lib/QtCore.framework/Headers/QObject \
		../../../Qt/5.11.0/clang_64/lib/QtCore.framework/Headers/QSize \
		../../../Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers/QQuickView \
		MainWindow.h \
		QmlCppBridge.h \
		moc_predefs.h \
		../../../Qt/5.11.0/clang_64/bin/moc
	/Users/semyon/Qt/5.11.0/clang_64/bin/moc $(DEFINES) --include ./moc_predefs.h -I/Users/semyon/Qt/5.11.0/clang_64/mkspecs/macx-clang -I/Users/semyon/Projects/VocalTrainer/QtVocalTrainer -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtGui.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQml.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtNetwork.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/usr/include -F/Users/semyon/Qt/5.11.0/clang_64/lib QmlCppBridge.h -o moc_QmlCppBridge.cpp

moc_qmlopenglitem.cpp: ../../../Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers/QQuickItem \
		qmlopenglitem.h \
		moc_predefs.h \
		../../../Qt/5.11.0/clang_64/bin/moc
	/Users/semyon/Qt/5.11.0/clang_64/bin/moc $(DEFINES) --include ./moc_predefs.h -I/Users/semyon/Qt/5.11.0/clang_64/mkspecs/macx-clang -I/Users/semyon/Projects/VocalTrainer/QtVocalTrainer -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQuick.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtGui.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtQml.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtNetwork.framework/Headers -I/Users/semyon/Qt/5.11.0/clang_64/lib/QtCore.framework/Headers -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/usr/include -F/Users/semyon/Qt/5.11.0/clang_64/lib qmlopenglitem.h -o moc_qmlopenglitem.cpp

compiler_moc_objc_header_make_all:
compiler_moc_objc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_rcc_clean compiler_moc_predefs_clean compiler_moc_header_clean 

