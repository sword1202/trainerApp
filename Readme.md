
# Build Instructions
## Build Instructions for Windows (MSVC 14.0 x32)
### Requirements:
- Visual Studio 2015.0 (VC140)
- Qt >= 5.10
- Python 2.7 (for aubio)
- bash (for aubio)

#### 1. Build aubio
- Install pip (https://github.com/BurntSushi/nfldb/wiki/Python-&-pip-Windows-installation)
- ```%PYTHONDIR%\\python.exe -m pip install wheel```
- ```SET PATH=%PATH_EXTRAS%;%PYTHONDIR%;%PYTHONDIR%\\Scripts;%PATH%```
- ```pip install --disable-pip-version-check --user --upgrade pip```
- ```pip install --upgrade setuptools```
- ```bash scripts/get_waf.sh```
- ```python waf configure build install --with-target-platform=win32 --prefix=./build/bin/release --build-type=release --notests --out=./build/release --check-c-compiler=msvc --msvc_targets="x86"```
- more info on https://github.com/aubio/aubio/issues/114
- required files: 
  - headers
  - aubio.lib (built as static lib by default)

#### 2. Download pre-built Boost (no need to build)
- go to https://sourceforge.net/projects/boost/files/boost-binaries/1.67.0/
- download and install boost_1_67_0-msvc-14.0-32.exe
- required files: 
  - headers
  - libboost_serialization-vc140-mt-x32-1_67.lib (static lib)

#### 3. Download pre-built GLEW (no need to build)
- download https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0-win32.zip/download
- required files: 
  - headers
  - glew32.lib, glew32s.lib
  - glew32.dll (copy to deploy dir)

#### 4. Build portaudio
- http://www.portaudio.com/download.html
- download latest http://www.portaudio.com/archives/pa_stable_v190600_20161030.tgz
- download ASIO https://github.com/chriskohlhoff/asio/releases
- copy ASIO SDK to portaudio\src\hostapi\asio - it will looks like portaudio\src\hostapi\asio\ASIOSDK
- open project in Visual Studio
- change "Configuration type" to "Static library"
- check if runtime library set to "MD" ("MDd" for Debug build)
- build form Visual Studio 
- required files: 
  - headers
  - portaudio.lib

#### 5. Build SoundTouch
- go to https://gitlab.com/soundtouch/soundtouch/tree/soundtouch-2.0.0
- download branch soundtouch-2.0.0
- open source\SoundTouch\SoundTouchDll.sln in Visual Studio
- go to project setting -> Code Generation
- change runtime library from MT to MD (MTd to MDd for Debug configuration)
- build from Visual Studio or using make-win.bat
- required files: 
  - headers
  - SoundTouch.lib

