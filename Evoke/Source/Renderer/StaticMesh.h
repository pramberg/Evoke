#pragma once
#include "Core/Core.h"
#include "Material.h"
#include "GraphicsContext.h"

namespace Evoke
{
	constexpr u32 NumVertexColors2 = 2;
	constexpr u32 NumUVSets2 = 6;

	template<typename T>
	class TVertexBuffer;

	template<typename T>
	class TIndexBuffer;

	struct DefaultVertex2
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		std::array<glm::vec4, NumVertexColors2> Color;
		std::array<glm::vec2, NumUVSets2> UV;
	};

	class StaticMesh
	{
	public:
		using VertexBufferType = std::vector<DefaultVertex2>;
		using IndexBufferType = std::vector<u32>;

		StaticMesh(StringView inMesh);
		StaticMesh(StringView inName, const VertexBufferType& inVertexBuffer, const IndexBufferType& inIndexBuffer);

		void SetMaterial(Material* inMaterial);
		Material* GetMaterial() { return mMaterial; }

		void Bind();
		void Render(GraphicsContext* inContext);

	private:
		TUniquePtr<TVertexBuffer<VertexBufferType>> mVertexBuffer = nullptr;
		TUniquePtr<TIndexBuffer<IndexBufferType>> mIndexBuffer = nullptr;

		class Material* mMaterial = nullptr;
		String mName;
	};
}