
#include "VTEManager.h"
#include "GLSceneDrawer.h"
#include "VxFile.h"
#include "MidiFileReader.h"

namespace VTE
{
	VTEManager::VTEManager()
		: _glSceneDrawer(nullptr)
		, _vxPlayer(nullptr)
	{
		/*
		MidiFileReader reader;
		std::vector<VxFile> v = reader.read("C:/dev/coding/Upwork/VocalTrainerOsX/VocalTrainer/melody.mid");
		for (size_t s = 0; s < v.size(); s++) {
			std::cout << v[s].getDurationInBits() << ' ' << v[s].getDurationInSeconds() << ' ' << v[s].getTrackEndSilenceBitsCount() << "\n";
			for (auto pitch : v[s].getPitches()) {
				std::cout << pitch.pitch.getName() << '_' << pitch.bitsCount << ' ';
			}
			std::cout << "\n\n";
		}
		*/
	}

	VTEManager::~VTEManager()
	{
		GLSceneDrawer* sceneDrawerPtr = static_cast<GLSceneDrawer*>(_glSceneDrawer);
		if (sceneDrawerPtr)
			delete sceneDrawerPtr;

		VxFile* vxPlayerPtr = static_cast<VxFile*>(_vxPlayer);
		if (vxPlayerPtr)
			delete vxPlayerPtr;
	}

	void VTEManager::initScene()
	{
		_glSceneDrawer = new GLSceneDrawer();
	}

	void VTEManager::drawScene(int iWidth, int iHeight)
	{
		if (_glSceneDrawer)
			((GLSceneDrawer*)_glSceneDrawer)->draw(iWidth, iHeight);
	}

	void VTEManager::setMoveOctaves(bool bValue)
	{
		if (_glSceneDrawer)
			((GLSceneDrawer*)_glSceneDrawer)->setMoveBetweenOctaves(bValue);
	}

	void VTEManager::PlayInVxFile(const char* fileName)
	{
		/* COMMENTED BY IVAN K.
		
		if (!_vxPlayer)
			_vxPlayer = new VxFile();

		((VxFile*)_vxPlayer)->load(fileName);
		((VxFile*)_vxPlayer)->play();

		*/
	}
}