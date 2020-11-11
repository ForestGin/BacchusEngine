#ifndef __Application__
#define __Application__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "Math.h" // Include MathGeoLib

#include <vector>
#include <list>
#include <string>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleGUI;
class MeshImporter;
class ModuleFileSystem;
class TextureImporter;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleGUI* gui = nullptr;
	MeshImporter* mesh_imp = nullptr;
	ModuleFileSystem* file_system = nullptr;
	TextureImporter* tex_imp = nullptr;

	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* link) const;

	const char* GetAppName() const;
	const char* GetOrgName() const;
	void ApplyAppName(const char* name);
	void ApplyOrgName(const char* name);
	string appName;
	string orgName;

	std::string GetPathName(std::string path);
	std::string GetPathDir(std::string path);

	list<char*> appLogs;

	//Frame rate
	uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	int						capped_ms = -1;
	int						framerateCap = 60; // Max amount of FPS
	int						totalBars = 100; // Number of bars that appear in the histogram
	std::vector<float>		fpsVec;
	std::vector<float>		msVec;

	float GetDT() const;
	float	dt;

	//JSON
	JsonImporter jsonImp;
	const char* jsonPath;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif __Application__