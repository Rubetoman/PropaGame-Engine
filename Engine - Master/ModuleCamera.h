#ifndef __MODULECAMERA_H__
#define __MODULECAMERA_H__

#include "Module.h"
#include "ModuleWindow.h"

#include "Point.h"
#include "Geometry/AABB.h"
#include <vector>

class GameObject;
class ComponentCamera;

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

	void		UpdateScreenSize();

	void		TranslateCameraInput();
	void		RotateCameraInput();
	void		CameraSpeedInput(float modifier);
	void		WheelInputTranslation(const fPoint& wheel_motion);
	void		FitCamera(const math::AABB &boundingBox);

public:

	// Editor Camera
	GameObject* editor_camera_go = nullptr;
	ComponentCamera* editor_camera_comp = nullptr;

	// Mouse 
	bool new_scroll = true;
	float mouse_sensitivity = 0.6;

	// Mesh
	math::AABB* BBtoLook = nullptr;

};

#endif /* __MODULECAMERA_H__ */

