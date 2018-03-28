
#include "VTEManager.h"
#include "GLSceneDrawer.h"
#include "VxFile.h"

namespace VTE
{
	VTEManager::VTEManager()
		: _glSceneDrawer(nullptr)
		, _vxPlayer(nullptr)
	{

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