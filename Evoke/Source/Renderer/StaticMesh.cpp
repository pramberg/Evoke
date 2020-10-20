#include "PCH.h"
#include "StaticMesh.h"

#include "assimp/Importer.hpp"
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace Evoke
{

	StaticMesh::StaticMesh(StringView inMesh) : mName(Filesystem::Filename(inMesh))
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(inMesh.data(), aiProcessPreset_TargetRealtime_MaxQuality);

		std::vector<DefaultVertex2> vertices;
		std::vector<u32> indices;

		if (!scene)
		{
			EV_LOG(LogTemp, ELogLevel::Error, "{}", importer.GetErrorString());
			return;
		}

		for (u32 i = 0; i < scene->mNumMeshes; i++)
		{
			auto* mesh = scene->mMeshes[i];

			vertices.reserve(vertices.size() + mesh->mNumVertices);
			indices.reserve(indices.size() + mesh->mNumFaces * 3LL);

			for (u32 v = 0; v < mesh->mNumVertices; v++)
			{
				const auto& pos = mesh->mVertices[v];
				const auto& nrm = mesh->mNormals[v];
				const auto& tan = mesh->mTangents[v];
				const auto& bit = mesh->mBitangents[v];

				std::array<glm::vec4, NumVertexColors2> colors{};
				for (u32 c = 0; c < NumVertexColors2; c++)
				{
					if (c < mesh->GetNumColorChannels())
					{
						const auto& color = mesh->mColors[c][v];
						colors[c] = { color.r, color.g, color.b, color.a };
					}
				}

				std::array<glm::vec2, NumUVSets2> uvs{};
				for (u32 c = 0; c < NumUVSets2; c++)
				{
					if (c < mesh->GetNumUVChannels())
					{
						const auto& uv = mesh->mTextureCoords[c][v];
						uvs[c] = { uv.x, uv.y };
					}
				}

				vertices.push_back({
					glm::vec3{ pos.x, pos.y, pos.z },
					glm::vec3{ nrm.x, nrm.y, nrm.z },
					glm::vec3{ tan.x, tan.y, tan.z },
					glm::vec3{ bit.x, bit.y, bit.z },
					std::move(colors),
					std::move(uvs)
					});
			}

			for (u32 f = 0; f < mesh->mNumFaces; f++)
			{
				auto face = mesh->mFaces[f];
				for (u32 j = 0; j < 3; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}
		}

		importer.FreeScene();
		
		mVertexBuffer = VertexBuffer::Create(vertices);
		mIndexBuffer = IndexBuffer::Create(indices);
	}

	StaticMesh::StaticMesh(StringView inName, const VertexBufferType& inVertexBuffer, const IndexBufferType& inIndexBuffer) : mName(inName)
	{
		mVertexBuffer = VertexBuffer::Create(inVertexBuffer);
		mIndexBuffer = IndexBuffer::Create(inIndexBuffer);
	}

	void StaticMesh::SetMaterial(Material* inMaterial)
	{
		mMaterial = inMaterial;
	}

	void StaticMesh::Bind()
	{
		mVertexBuffer->Bind();
	}

	void StaticMesh::Render(GraphicsContext* inContext)
	{
		if (!mMaterial)
			return;
		
		inContext->BeginEvent(mName + " " + mMaterial->Name());

		inContext->SetFaceCullingMethod(mMaterial->FaceCulling());
		mMaterial->Bind();
		for (auto [slot, texture] : mMaterial->Textures())
			texture->Bind(slot);

		mVertexBuffer->Bind();
		inContext->DrawIndexed(mIndexBuffer->Size());
		inContext->EndEvent();
	}

}

