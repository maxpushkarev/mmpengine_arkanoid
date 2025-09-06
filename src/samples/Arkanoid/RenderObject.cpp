#include <Arkanoid/RenderObject.hpp>
#include <Frontend/Mesh.hpp>
#include <Frontend/Shader.hpp>
#include <Core/Node.hpp>

namespace Sample::Arkanoid
{
	MeshRendererObject::MeshRendererObject(const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext)
	{
		auto defaultNode = std::make_shared<MMPEngine::Core::Node>();
		//detached instances will be skipped by culling
		globalContext->math->RotationAroundAxis(defaultNode->localTransform.rotation, MMPEngine::Core::Vector3Float{ 0.0f, 1.0f, 0.0f }, MMPEngine::Core::Math::ConvertDegreesToRadians(180));
		globalContext->math->CalculateLocalToWorldSpaceMatrix(_defaultInstanceData.local2World, defaultNode);
	}

	const VisualObject::InstanceData& MeshRendererObject::GetDefaultInstanceData() const
	{
		return _defaultInstanceData;
	}
}