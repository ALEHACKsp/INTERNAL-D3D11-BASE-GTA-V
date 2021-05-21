#pragma once 

enum bone_ids : int
{
	head = 0,
	lfoot = 1,
	rfoot = 2,
	lhand = 5,
	rhand = 6,
	neck = 7,
	pelvis = 8
};

namespace offsets
{
	// 18.3.2021 | GameVersion = ""
	constexpr unsigned int world = 0x2501cf0;
	constexpr unsigned int replayinterface = 0x1ef4998;
	constexpr unsigned int viewport = 0x1fa39c0;

	namespace ped
	{
		constexpr unsigned int pedType = 0x10b8;
		constexpr unsigned int pedInfo = 0x10c8;
		constexpr unsigned int weapon = 0x10d8;
		constexpr unsigned int vehicle = 0x1520;
	}

	namespace pedInfo
	{
		constexpr unsigned int name = 0xa4;
		constexpr unsigned int wantedLevel = 0x888;
	}
}

namespace functions
{
	auto getEntityListPointer() -> uintptr_t {
		static auto base = (uintptr_t)GetModuleHandleA(NULL);
		if (!base)
			return 0x0;

		unsigned __int64 interface_list = *reinterpret_cast<unsigned __int64*>(base + offsets::replayinterface);
		if (!isValidPtr(interface_list))
			return 0x0;

		unsigned __int64 ped_interface = *reinterpret_cast<unsigned __int64*>(interface_list + 0x18);
		if (!isValidPtr(ped_interface))
			return 0x0;

		return *reinterpret_cast<unsigned __int64*>(ped_interface + 0x100);
	}

	auto getLocalPlayer() -> uintptr_t {
		static auto pedFactory = *(uintptr_t*)((uintptr_t)GetModuleHandleA(NULL) + offsets::world);
		if (!isValidPtr(pedFactory))
			return 0x0;

		return *(uintptr_t*)(pedFactory + 0x8);
	}

	auto get_bone_transform(uintptr_t ped, int bone_id) -> vec3 {
		auto boneTransform = *reinterpret_cast<D3DXVECTOR3*>(ped + 0x430 + bone_id * 0x10);
		auto boneMatrix = *reinterpret_cast<D3DXMATRIX*>(ped + 0x60);
		D3DXVECTOR4 transform;
		D3DXVec3Transform(&transform, &boneTransform, &boneMatrix);
		return { transform.x, transform.y, transform.z };
	}

	auto world_to_screen(D3DXMATRIX inMatrix, const vec3 world, vec3* screen) -> bool {
		static float width = 1920;
		static float height = 1080;
		static float x_temp = width / 2;
		static float y_temp = height / 2;

		D3DXMATRIX matrix{};
		D3DXMatrixTranspose(&matrix, &inMatrix);

		const D3DXVECTOR4 vec_x = D3DXVECTOR4(matrix._21, matrix._22, matrix._23, matrix._24);
		const D3DXVECTOR4 vec_y = D3DXVECTOR4(matrix._31, matrix._32, matrix._33, matrix._34);
		const D3DXVECTOR4 vec_z = D3DXVECTOR4(matrix._41, matrix._42, matrix._43, matrix._44);

		vec3 screen_pos =
		{
			static_cast<float>(vec_x.x * world.x) + static_cast<float>(vec_x.y * world.y) + static_cast<float>(vec_x.z * world.z) + vec_x.w,
			static_cast<float>(vec_y.x * world.x) + static_cast<float>(vec_y.y * world.y) + static_cast<float>(vec_y.z * world.z) + vec_y.w,
			static_cast<float>(vec_z.x * world.x) + static_cast<float>(vec_z.y * world.y) + static_cast<float>(vec_z.z * world.z) + vec_z.w
		};

		if (screen_pos.z <= 0.1f)
			return false;

		screen_pos.z = 1.0f / screen_pos.z;
		screen_pos.x *= screen_pos.z;
		screen_pos.y *= screen_pos.z;

		*screen =
		{
			screen_pos.x + x_temp + float(0.5f * screen_pos.x * width + 0.5f),
			y_temp - float(0.5f * screen_pos.y * height + 0.5f),
			0
		};

		return true;
	}
}