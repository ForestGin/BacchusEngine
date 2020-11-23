#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(0.99f, 0.0f, -0.11f);
	Y = vec3(-0.04f, 0.92f, -0.37f);
	Z = vec3(0.18f, 0.34f, 0.92f);

	Position = vec3(17.92f, 59.18f, 93.58f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	mainCam = new ComponentCamera();
	activeCam = new ComponentCamera();
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Start()
{
	LOG_C("Loading Camera 3D");

	cameraGO = App->scene_intro->CreateGO("Camera", App->scene_intro->GOroot);
	cameraGO->CreateComponent(COMPONENT_TYPE::CAMERA);

	return true;
}

bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	RELEASE(mainCam);

	return true;
}

update_status ModuleCamera3D::Update(float dt)
{
	if (isOnConfiguration || isOnConsole || isOnHierarchy || isOnInspector || isOnState || isOnResources)
	{
		// Trying to focus while hovering a menu
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && !focusError)
		{
			LOG_C("WARNING: You have to be inside the scene to focus the camera to the selected GameObject");
			focusError = true;
		}

		return UPDATE_CONTINUE;
	}

	focusError = false;

	vec3 newPos(0,0,0);
	float speed = 3.0f * App->GetDT() * WASDValue;

	// Focus
	if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		if (App->scene_intro->GOselected != nullptr)
		{
			GameObject* GOselected = App->scene_intro->GOselected;

			Focus(GOselected);
		}
	}

	// Double the speed
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = speed * 2;

	// Mouse motion

	// Mouse Picking
	/*if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		GameObject* pick = MousePicking();

		if (pick != nullptr)
			App->scene_intro->GOselected = pick;
		else
			App->scene_intro->GOselected = nullptr;
	}*/

	// Camera Orbit
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position += newPos;
			Reference += newPos;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
	}

	// Look around from position
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		
		float Sensitivity = 0.25f;

		Position += newPos;
		Reference += newPos;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Zoom In & Zoom Out
	if (App->input->GetMouseZ() != 0)
	{
		vec3 Distance = Position - Reference;
		vec3 newPos = { 0,0,0 };

		if (App->input->GetMouseZ() > 0 || App->input->GetMouseZ() < 0)
		{
			newPos -= Z * App->input->GetMouseZ() * length(Distance) / (1/zoomValue*10);
			Position += newPos;
		}

		Position += newPos;
	}

	// Wheel Movement
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dx != 0 || dy != 0)
		{
			newPos -= Y * dy * App->GetDT() * wheelSpeedValue;
			newPos += X * dx * App->GetDT() * wheelSpeedValue;
		}

		Position += newPos;
		Reference += newPos;
	}

	// Recalculate matrix
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}

void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::GoInitialPos()
{
	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(1.0f, 0.5f, 7.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	CalculateViewMatrix();
}

void ModuleCamera3D::Focus(GameObject* go)
{
	if (go != nullptr)
	{
		float3 pos = go->aabb.CenterPoint();

		Reference.x = pos.x;
		Reference.y = pos.y;
		Reference.z = pos.z;

		LookAt(Reference);
	}
}

ComponentCamera* ModuleCamera3D::GetActiveCamera()
{
	return activeCam;
}

const Frustum& ModuleCamera3D::GetActiveFrustum() const
{
	return activeCam->frustum;
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

bool ModuleCamera3D::Intersects(const AABB& box) const
{
	return activeCam->Intersect(box);
}

float3 ModuleCamera3D::GetPosition() const
{
	return activeCam->frustum.pos;
}

float* ModuleCamera3D::GetView() const
{
	return activeCam->GetView().ptr();
}

float* ModuleCamera3D::GetProjection() const
{
	return activeCam->GetProjection().ptr();
}

bool* ModuleCamera3D::GetProjectionBool() const
{
	return &activeCam->update_frustum;
}

GameObject* ModuleCamera3D::MousePicking(float3* position) const
{
	float posX = (float)App->input->GetMouseX() / (float)App->window->GetWidth() * 2.f - 1.f;
	float posY = -((float)App->input->GetMouseY() / (float)App->window->GetHeight() * 2.f - 1.f);

	LineSegment projection = activeCam->NearSegment(posX, posY);

	float distance;

	GameObject* hitGO = App->scene_intro->MousePicking(projection, distance, true);

	if (hitGO != nullptr && position != nullptr)
	{
		*position = projection.GetPoint(distance);
	}

	return hitGO;
}