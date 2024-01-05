#pragma once

namespace Toffee {
	// ID of a game object.
	// Note that game objects in separate scenes may have the same ID.
	typedef uint32_t GameObject;
	
	// Shape of a physics collider.
	enum class PhysicsShape {
		BOX,
		SPHERE,
		CAPSULE,
		PLANE
	};
	// Defines whether a physics body is static or dynamic.
	enum class PhysicsType {
		STATIC,
		DYNAMIC
	};
	
}

namespace Toffee::Components {
	// Transform component.
	// Altered by world and physics systems, used by renderer.
	struct Transform {
		float scale_x = 1.f;
		float scale_y = 1.f;
		float scale_z = 1.f;

		float matrix[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	};

	struct Mesh {
		std::vector<float> vertex, normals, texCoords;
	};
	// Used by renderer.
	struct Renderable {
		Mesh* mesh = nullptr;
		bool triangles = true;
		void* texture;
		float ambient[4] = { 0.5f, 0.5f, 0.5f, 1.f };
		float diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
	};


	// Used by renderer.
	struct Light {
		bool has_position = true;
		float ambient[4] = { 0.5f, 0.5f, 0.5f, 1.f };
		float diffuse[4] = { 1.f, 1.f, 1.f, 1.f };
		float spot_dir[3] = { 0, 0, 0 };
		float spot_cutoff = -1;
		float spot_exp = -1;
	};

	// Contains pointer for accessing the physics body.
	struct PhysicsBody {
		void* body = nullptr;
	};
}
