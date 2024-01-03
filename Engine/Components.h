#pragma once

namespace Engine {
	typedef uint32_t GameObject;
	
	enum class PhysicsShape {
		BOX,
		SPHERE,
		CAPSULE,
		PLANE
	};
	enum class PhysicsType {
		STATIC,
		DYNAMIC
	};
	
}

namespace Engine::Components {
	struct Transform {
		/*float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float w = 0.f;
		float rotx = 0.f;
		float roty = 0.f;
		float rotz = 0.f;*/
		float scale_x = 1.f;
		float scale_y = 1.f;
		float scale_z = 1.f;

		// rotation override and rotation multiplier

		float matrix[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	};

	struct Mesh {
		std::vector<float> vertex, normals, texCoords;
	};

	struct Renderable {
		Mesh* mesh = nullptr;
		bool triangles = true;
		void* texture;
		float ambient[4] = { 0.5f, 0.5f, 0.5f, 1.f };
		float diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
	};

	struct Light {
		bool has_position = true;
		float ambient[4] = { 0.5f, 0.5f, 0.5f, 1.f };
		float diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		float spot_dir[3] = { 0, 0, 0 };
		float spot_cutoff = -1;
		float spot_exp = -1;
	};

	struct PhysicsBody {
		void* body = nullptr;
	};
}
