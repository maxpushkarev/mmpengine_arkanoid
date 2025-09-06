#pragma once
#include <memory>
#include <Arkanoid/Object.hpp>
#include <Arkanoid/VisualObject.hpp>
#include <Core/Mesh.hpp>
#include <Core/Shader.hpp>
#include <Frontend/Buffer.hpp>

namespace Sample::Arkanoid
{
	class MeshRendererObject : public BaseObject
	{
	public:

		MeshRendererObject(const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext);
		const VisualObject::InstanceData& GetDefaultInstanceData() const;

		std::shared_ptr<MMPEngine::Core::Mesh::Renderer> renderer;
		std::shared_ptr<MMPEngine::Core::ShaderPack> shaderPack;
		std::shared_ptr<MMPEngine::Core::Shader> vertexShader;
		std::shared_ptr<MMPEngine::Core::Shader> pixelShader;
		std::shared_ptr<MMPEngine::Frontend::StructuredUploadBuffer<VisualObject::InstanceData>> instanceBuffer;
		std::shared_ptr<MMPEngine::Core::BaseTask> resetInstanceBufferTask;
	private:
		VisualObject::InstanceData _defaultInstanceData{};
	};

	class GameRendererObject : public BaseObject
	{
	public:
		std::shared_ptr<MMPEngine::Core::BaseTask> rootTask;
	};
}
