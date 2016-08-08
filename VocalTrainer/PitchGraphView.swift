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
    let pitchReader:PitchInputReaderObjC

    required init?(coder: NSCoder) {
        pitchReader = PitchInputReaderObjC(sampleSize: 2400)
        pitchReader.start()
        super.init(coder: coder)
    }

    override func prepareOpenGL() {
        window!.makeFirstResponder(self)
        glClearColor(0, 0, 0, 0)

        let timer = NSTimer(timeInterval: 1.0/60.0, target:self, selector: #selector(PitchGraphView.idle), userInfo:nil, repeats:true);
        NSRunLoop.currentRunLoop().addTimer(timer, forMode:NSDefaultRunLoopMode);
}

    func idle(timer:NSTimer) {
        if(!NSApplication.sharedApplication().hidden) {
            display()
        }
    }

    override func drawRect(dirtyRect:NSRect) {
        glClear(GLenum(GL_COLOR_BUFFER_BIT));
        drawObject();
        glFlush();
    }

    func drawObject() {
        glColor3f(1.0, 0.85, 0.35)
        if let pitch = pitchReader.getLastDetectedPitch() {
            glBegin(GLenum(GL_TRIANGLES))
            glVertex3f(  0.0, pitch.getFrequency() / 2000.0, 0.0)
            glVertex3f( -0.2, -0.3, 0.0)
            glVertex3f(  0.2, -0.3 ,0.0)
            glEnd()
        }
    }
}
