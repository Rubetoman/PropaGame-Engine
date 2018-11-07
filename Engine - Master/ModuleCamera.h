#ifndef __ModuleCamera_h__
#define __ModuleCamera_h__

#include "Camera.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Point.h"
#include "GL/glew.h"
#include "SDL.h"
#include "MathGeoLib.h"
#include "SDL/include/SDL.h"
#include <vector>

class Camera;

class ModuleCamera : public Module
{
	enum CameraRotation 
	{
		PositivePitch,
		NegativePitch,
		PositiveYaw,
		NegativeYaw
	};

public:
	ModuleCamera();
	~ModuleCamera();

	bool            Init();
	update_status	PreUpdate();
	update_status   Update();
	bool            CleanUp();

	void			UpdateScreenSize();

	void			TranslateCameraInput();
	void			RotateCameraInput();
	void			CameraSpeedInput(float modifier);
	void			MouseUpdate(const iPoint& mouse_position);

	// Camera
	Camera* mainCamera = nullptr;			// Default camera
	std::vector<bool> activeCameras;		// Vector of boleans to know which cameras are active
	std::vector<Camera*> cameras;			// Vector with all the cameras on the scene

	// Mouse 
	bool firstMouse = true;
	float lastX = SCREEN_WIDTH / 2;
	float lastY = SCREEN_HEIGHT / 2;

};

#endif /* __ModuleCamera_h__ */

