#pragma once

class CPlayerEsp {
private:
	int checkValidCoordinates(D3DXMATRIX viewMatrix, uintptr_t ped, vec3* bones) {
		static int spine[3] = { bone_ids::head, bone_ids::neck, bone_ids::pelvis };
		static int body[6] = { bone_ids::lhand, bone_ids::neck, bone_ids::rhand, bone_ids::lfoot, bone_ids::pelvis, bone_ids::rfoot };

		int validCheck = 0;
		for (unsigned int b = 0; b < 9; b++)
			if (functions::world_to_screen(viewMatrix, functions::get_bone_transform(ped, b < 3 ? spine[b] : body[b - 3]), &bones[b]))
				validCheck++;

		return validCheck;
	}

	void drawSkeleton(vec3* bones) {
		static int spine[3] = { bone_ids::head, bone_ids::neck, bone_ids::pelvis };
		static int body[6] = { bone_ids::lhand, bone_ids::neck, bone_ids::rhand, bone_ids::lfoot, bone_ids::pelvis, bone_ids::rfoot };

		for (unsigned int i = 0; i < 8; i++) {
			vec3 bone_child{}, bone_parent{};

			if (body[i] == bone_ids::rhand || body[i] == bone_ids::rfoot || spine[i] == bone_ids::pelvis)
				continue;

			bone_parent = bones[i];
			bone_child = bones[i + 1];

			ImGui::GetOverlayDrawList()->AddLine(ImVec2{ bone_child.x, bone_child.y }, ImVec2{ bone_parent.x, bone_parent.y }, ImColor(255, 255, 255, 255));
		}
	}

public:
	void drawPlayers() {
		static auto viewPort = *(uintptr_t*)((uintptr_t)GetModuleHandleA(NULL) + offsets::viewport);
		auto viewMatrix = *(D3DXMATRIX*)(viewPort + 0x24c);
		for (unsigned int i = 0; i < 256; i++) {
			auto ped = *(uintptr_t*)(functions::getEntityListPointer() + (0x10 * i));
			if (!isValidPtr(ped))
				continue;

			auto pedType = *reinterpret_cast<int*>(ped + offsets::ped::pedType) << 11 >> 25;
			if (pedType != 2)
				continue;
			
			auto pedInfo = *reinterpret_cast<uintptr_t*>(ped + offsets::ped::pedInfo);
			if (!isValidPtr(pedInfo))
				continue;

			vec3 bones[9];
			auto validCheck = checkValidCoordinates(viewMatrix, ped, bones);
			if (validCheck != 9)
				continue;

			drawSkeleton(bones);				
		}
	}
};

extern CPlayerEsp* playerEsp = new CPlayerEsp();