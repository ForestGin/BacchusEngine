#include "ComponentBillboard.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Application.h"

ComponentBillboard::ComponentBillboard(GameObject* GO) : Component(COMPONENT_TYPE::BILLBOARD, GO)
{
	type = COMPONENT_TYPE::BILLBOARD;
	billboardtype = BILLBOARD_TYPE::NONE;

	GO->componentsList.push_back(this);
}

ComponentBillboard::~ComponentBillboard()
{
	/*gameObject->components.remove(this);*/
}

void ComponentBillboard::DrawInspector()
{
	ImGui::Spacing();

	GameObject* go = App->scene_intro->GOselected;

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (ImGui::CollapsingHeader("Billboard"))
	{
		ImGui::Spacing();

		if (ImGui::BeginMenu("Select billboard"))
		{
			if (ImGui::MenuItem("Screen")) {
				billboardtype = BILLBOARD_TYPE::SCREEN;

			}
			if (ImGui::MenuItem("Wold")) {
				billboardtype = BILLBOARD_TYPE::WORLD;

			}
			if (ImGui::MenuItem("Axial")) {
				billboardtype = BILLBOARD_TYPE::AXIAL;

			}
			ImGui::EndMenu();
		}

		if (billboardtype == BILLBOARD_TYPE::AXIAL)
		{
			if (ImGui::BeginMenu("Choose Axis"))
			{

				if (ImGui::MenuItem("X")) {
					billboardaxis = BILLBOARD_AXIS::X;

				}
				if (ImGui::MenuItem("Y")) {
					billboardaxis = BILLBOARD_AXIS::Y;

				}
				if (ImGui::MenuItem("Z")) {
					billboardaxis = BILLBOARD_AXIS::Z;

				}
				ImGui::EndMenu();
			}
		}
	}
}

bool ComponentBillboard::Update()
{
	if (billboardtype != BILLBOARD_TYPE::NONE) {

		float3 x, y, z;

		switch (billboardtype)
		{

		case BILLBOARD_TYPE::SCREEN:

			/*z = -App->camera->compCamera->frustum.front;
			y = App->camera->compCamera->frustum.up;

			x = y.Cross(z);*/

			break;

		case BILLBOARD_TYPE::WORLD:

			/*z = (App->camera->cameraGO->transform->GetGlobalPos() - gameObject->transform->GetGlobalPos()).Normalized();
			y = App->camera->compCamera->frustum.up;

			x = y.Cross(z);

			y = z.Cross(x);*/

			break;

		case BILLBOARD_TYPE::AXIAL:

			/*float3 dist = (App->camera->cameraGO->transform->GetGlobalPos() - gameObject->transform->GetGlobalPos()).Normalized();*/
			switch (billboardaxis) {

			case BILLBOARD_AXIS::X:

				/*z = gameObject->transform->GetGlobalRotation() * float3(0, 0, 1);
				y = z.Cross(dist);

				x = y.Cross(z);*/

				break;
			case BILLBOARD_AXIS::Y:

				/*y = gameObject->transform->GetGlobalRotation() * float3(0, 1, 0);
				x = y.Cross(dist);

				z = x.Cross(y);*/

				break;
			case BILLBOARD_AXIS::Z:

				/*x = gameObject->transform->GetGlobalRotation() * float3(1, 0, 0);
				y = dist.Cross(x);

				z = x.Cross(y);*/

				break;
			}
		}
	}

	return true;
}