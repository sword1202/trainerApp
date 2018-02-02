#pragma once


#if defined(VTE_DLL_BUILD)
#define VTEMANAGER_API __declspec(dllexport)
#elif defined(VTE_DLL)
#define VTEMANAGER_API __declspec(dllimport)
#endif


#if !defined VTEMANAGER_API
#define VTEMANAGER_API
#endif

namespace VTE
{
	class VTEMANAGER_API VTEManager
	{
		void* _vxPlayer;
		void* _glSceneDrawer;

	public:
		VTEManager();
		~VTEManager();

		void initScene();
		void drawScene(int width, int height);
		void setMoveOctaves(bool bValue);

		void PlayInVxFile(const char* fileName);
	};
}
