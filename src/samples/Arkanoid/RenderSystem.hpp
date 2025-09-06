#pragma once
#include <Arkanoid/RenderObject.hpp>
#include <Arkanoid/VisualObject.hpp>
#include <Arkanoid/System.hpp>
#include <Core/Context.hpp>
#include <unordered_set>

namespace Sample::Arkanoid
{
	class RenderSystem final :
		public MultiObjectSystem<MeshRendererObject, GameRendererObject, VisualObject>,
		public BaseUpdateSystem, public BaseRenderSystem
	{
	public:
		RenderSystem(
			WorldService* world, const IConfigService* config,
			const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext,
			const std::shared_ptr<MMPEngine::Core::BaseStream>& stream
		);
		void Update(std::float_t dt) override;
		void OnRender() override;
		void OnViewportUpdated() override;
	private:

		void CreateMeshRenderer();
		void UpdateInstances();
		void ClearGameRenderers();
		void CreateGameRenderer();

		std::shared_ptr < MMPEngine::Core::GlobalContext> _globalContext;
		std::shared_ptr<MMPEngine::Core::BaseStream> _stream;
		std::unordered_set<std::size_t> _tmpFreeInstanceIndices;
	};
}
