#pragma once

#include "OpenGL.h"
#include "../VocalTrainerEngine/VTEManager.h"
#include <exception>

namespace VocalTrainer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace OpenGLForm;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		VTE::VTEManager* vteManager;
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			vteManager = new VTE::VTEManager();	
			OpenGL = gcnew COpenGL(vteManager, this->scenePanel, this->scenePanel->Width, this->scenePanel->Height);
			OpenGL->setMoveBetweenOctaves(this->chkMoveOctaves->Checked);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;

				if(vteManager)
					delete vteManager;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::CheckBox^  chkMoveOctaves;
	private: System::Windows::Forms::Panel^  scenePanel;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  timer1;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		OpenGLForm::COpenGL ^ OpenGL;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->chkMoveOctaves = (gcnew System::Windows::Forms::CheckBox());
			this->scenePanel = (gcnew System::Windows::Forms::Panel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->chkMoveOctaves);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Top;
			this->panel1->Location = System::Drawing::Point(0, 0);
			this->panel1->Margin = System::Windows::Forms::Padding(0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(832, 50);
			this->panel1->TabIndex = 0;
			// 
			// chkMoveOctaves
			// 
			this->chkMoveOctaves->AutoSize = true;
			this->chkMoveOctaves->Checked = true;
			this->chkMoveOctaves->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkMoveOctaves->Location = System::Drawing::Point(16, 15);
			this->chkMoveOctaves->Margin = System::Windows::Forms::Padding(4);
			this->chkMoveOctaves->Name = L"chkMoveOctaves";
			this->chkMoveOctaves->Size = System::Drawing::Size(166, 20);
			this->chkMoveOctaves->TabIndex = 0;
			this->chkMoveOctaves->Text = L"Move between octaves";
			this->chkMoveOctaves->UseVisualStyleBackColor = true;
			// 
			// scenePanel
			// 
			this->scenePanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->scenePanel->Location = System::Drawing::Point(0, 50);
			this->scenePanel->Name = L"scenePanel";
			this->scenePanel->Size = System::Drawing::Size(832, 493);
			this->scenePanel->TabIndex = 1;
			this->scenePanel->Resize += gcnew System::EventHandler(this, &Form1::scenePanel_Resize);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(832, 543);
			this->Controls->Add(this->scenePanel);
			this->Controls->Add(this->panel1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Vocal Trainer";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		{
			UNREFERENCED_PARAMETER(sender);
			UNREFERENCED_PARAMETER(e);

			OpenGL->setMoveBetweenOctaves(this->chkMoveOctaves->Checked);
			OpenGL->Render(this->scenePanel->Width, this->scenePanel->Height);
			OpenGL->SwapOpenGLBuffers();
		}

		private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			try
			{
				CheckAudioDeviceAvailability();
			}
			catch (std::exception ex)
			{
				MessageBox::Show(gcnew System::String(ex.what()));
			}

			const char* path = "1.json";
			if (vteManager)
			{
				try
				{
					vteManager->PlayInVxFile(path);
				}
				catch (std::exception ex)
				{
					//MessageBox::Show(gcnew System::String(ex.what()));
				}
			}
		}
	
		private: System::Void scenePanel_Resize(System::Object^  sender, System::EventArgs^  e) 
		{
			if (OpenGL)
				OpenGL->ResizeWindow(this->scenePanel->Width, this->scenePanel->Height);
		}

		private: System::Void CheckAudioDeviceAvailability()
		{
			if (waveInGetNumDevs() == 0)
				throw std::exception("No sound card installed !");


			WAVEINCAPS waveInCaps;
			if (waveInGetDevCaps(0,
				&waveInCaps,
				sizeof(WAVEINCAPS))
				!= MMSYSERR_NOERROR)
			{
				throw std::exception("Cannot determine sound card capabilities !");
			}

			if ((waveInCaps.dwFormats & WAVE_FORMAT_PCM) == 0)
			{
				throw std::exception("PCM format not supported");
			}
		}
	};
}

