#pragma once

#include "OpenGL.h"

using namespace System::Windows::Forms;

namespace OpenGLForm
{
	COpenGL::COpenGL(VTE::VTEManager* pManager, System::Windows::Forms::Panel ^ parentForm, GLsizei iWidth, GLsizei iHeight)
	{
		CreateParams^ cp = gcnew CreateParams;

		// Set the position on the form
		cp->X = 0;
		cp->Y = 0;
		cp->Height = iHeight;
		cp->Width = iWidth;

		// Specify the form as the parent.
		cp->Parent = parentForm->Handle;

		// Create as a child of the specified parent and make OpenGL compliant (no clipping)
		cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		// Create the actual window
		this->CreateHandle(cp);

		m_hDC = GetDC((HWND)this->Handle.ToPointer());

		if (m_hDC)
		{
			MySetPixelFormat(m_hDC);
			//ReSizeGLScene(iWidth, iHeight);
			//InitGL();
		}

		vteManager = pManager;
		if(vteManager)
			vteManager->initScene();
	}


	System::Void COpenGL::Render(GLsizei iWidth, GLsizei iHeight)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		if(vteManager)
			vteManager->drawScene(iWidth, iHeight);
	}

	System::Void COpenGL::SwapOpenGLBuffers(System::Void)
	{
		SwapBuffers(m_hDC);
	}

	void COpenGL::setMoveBetweenOctaves(bool bValue)
	{
		if (vteManager)
			vteManager->setMoveOctaves(bValue);
	}
}