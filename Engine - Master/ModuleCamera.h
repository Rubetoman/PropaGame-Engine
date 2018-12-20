#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Camera.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "WindowScene.h"
#include "ModuleTime.h"

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

	void		DeleteCamera(GameObject* go);

	void		UpdateScreenSize();

	void		TranslateCameraInput();
	void		RotateCameraInput();
	void		CameraSpeedInput(float modifier);
	void		MouseInputTranslation(const fPoint& mouse_position);
	void		MouseInputRotation(const fPoint& mouse_position);
	void		WheelInputTranslation(const fPoint& wheel_motion);
	void		FitCamera(const AABB &boundingBox);

	// Camera
	Camera* mainCamera = nullptr;			// Default camera
	std::vector<GameObject*> cameras;		// Vector with all the cameras on the scene

	// Mouse 
	bool new_click = true;
	bool new_scroll = true;
	float mouse_sensitivity = 0.6;
	float last_x = SCREEN_WIDTH / 2;
	float last_y = SCREEN_HEIGHT / 2;

	// Mesh
	AABB* BBtoLook = nullptr;

};

#endif /* __MODULECAMERA_H__ */

