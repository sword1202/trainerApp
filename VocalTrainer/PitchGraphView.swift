//
// Created by Semyon Tikhonenko on 8/8/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

import Foundation
import Cocoa
import OpenGL
import GLKit
import GLUT

class PitchGraphView : AppKit.NSOpenGLView {
    required init?(coder: NSCoder) {
        super.init(coder: coder)
    }

    override func drawRect(dirtyRect:NSRect) {
        glClearColor(0, 0, 0, 0);
        glClear(GLenum(GL_COLOR_BUFFER_BIT));
        drawObject();
        glFlush();
    }

    func drawObject() {
        glColor3f(1.0, 0.85, 0.35)
        glBegin(GLenum(GL_TRIANGLES))
            glVertex3f(  0.0,  0.6, 0.0)
            glVertex3f( -0.2, -0.3, 0.0)
            glVertex3f(  0.2, -0.3 ,0.0)
        glEnd()
    }
}
