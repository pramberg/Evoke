#pragma once
#include "Core/Core.h"
#include "glm/glm.hpp"

namespace Evoke
{
	class Vec3f : public glm::vec3
	{
	public:
		using glm::vec3::vec3;

		static f32 Dot(const Vec3f& inA, const Vec3f& inB) { return glm::dot(static_cast<glm::vec3>(inA), static_cast<glm::vec3>(inB)); }
		static Vec3f Cross(const Vec3f& inA, const Vec3f& inB) { return glm::cross(static_cast<glm::vec3>(inA), static_cast<glm::vec3>(inB)); }

		static constexpr Vec3f Forward() { return { 0.0f, 0.0f, 1.0f }; }
		static constexpr Vec3f Up() { return { 0.0f, 1.0f, 0.0f }; }
		static constexpr Vec3f Right() { return { 1.0f, 0.0f, 0.0f }; }
		static constexpr Vec3f Zero() { return Vec3f(0.0f); }
		static constexpr Vec3f One() { return Vec3f(1.0f); }
	};

}