// VoiceTrainer.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"
// #include "../../VocalTrainer/VxPlayer/MidiFileReader.h"

using namespace VocalTrainer;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Debugging MIDI functionality
//	MidiFileReader reader;
//	std::vector<VxFile> s = reader.read("TakeOnMe.mid");

//	ADD writeToStream(cout);

	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
