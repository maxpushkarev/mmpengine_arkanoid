#include <Arkanoid/WorldService.hpp>
#include <Arkanoid/ConfigService.hpp>
#include <Arkanoid/RenderSystem.hpp>
#include <Frontend/Geometry.hpp>
#include <Frontend/Mesh.hpp>
#include <Frontend/Shader.hpp>
#include <Frontend/Screen.hpp>
#include <Frontend/Camera.hpp>
#include <Frontend/Texture.hpp>
#include <Frontend/DrawCall.hpp>
#include <Frontend/Buffer.hpp>
#include <Core/Node.hpp>
#include <cassert>


namespace Sample::Arkanoid
{
	RenderSystem::RenderSystem(
		WorldService* world, const IConfigService* config,
		const std::shared_ptr<MMPEngine::Core::GlobalContext>& globalContext,
		const std::shared_ptr<MMPEngine::Core::BaseStream>& stream) : BaseSystem(world, config), _globalContext(globalContext), _stream(stream)
	{
	}

	void RenderSystem::CreateMeshRenderer()
	{
		const auto& meshRenderers = this->ObjectSystem<MeshRendererObject>::_objectsMap;

		if (meshRenderers.empty())
		{
			auto meshRendererObject = std::make_unique<MeshRendererObject>(_globalContext);

			const auto pack = std::make_shared<MMPEngine::Frontend::ShaderPack>(_globalContext, std::filesystem::path("Arkanoid.json"));
			const auto vs = pack->Unpack("VertexQuad");
			const auto ps = pack->Unpack("PixelQuad");

			auto quadProto = MMPEngine::Frontend::Geometry::Generate<MMPEngine::Frontend::Geometry::PrimitiveType::Quad>();
			const auto quadMesh = std::make_shared<MMPEngine::Frontend::Mesh>(_globalContext, std::move(quadProto));
			const auto quadMeshRendererNode = std::make_shared<MMPEngine::Core::Node>();

			const auto quadRenderer = std::make_shared<MMPEngine::Frontend::Mesh::Renderer>(
				_globalContext,
				MMPEngine::Core::Mesh::Renderer::Settings{
					{
						false,
						std::vector<MMPEngine::Core::GeometryPrototype::VertexAttribute> {
							{MMPEngine::Core::VertexBufferPrototype::Semantics::Position, 0 }
						}
					},
					{0}
				},
				quadMesh,
				quadMeshRendererNode
			);

			meshRendererObject->shaderPack = pack;
			meshRendererObject->vertexShader = vs;
			meshRendererObject->pixelShader = ps;
			meshRendererObject->renderer = quadRenderer;


			{
				const auto executor = _stream->CreateExecutor();

				_stream->Schedule(pack->CreateInitializationTask());
				_stream->Schedule(vs->CreateInitializationTask());
				_stream->Schedule(ps->CreateInitializationTask());


				_stream->Schedule(quadRenderer->GetMesh()->CreateInitializationTask());
				_stream->Schedule(quadRenderer->CreateInitializationTask());
			}

			_world->AddObject(std::move(meshRendererObject));
		}
	}

	void RenderSystem::UpdateInstances()
	{
		const auto& meshRenderers = this->ObjectSystem<MeshRendererObject>::_objectsMap;
		const auto& visualObjects = this->ObjectSystem<VisualObject>::_objectsMap;

		for (const auto& mr : meshRenderers)
		{
			if (visualObjects.size() > mr.second->renderer->GetDynamicSettings().instancesCount)
			{
				mr.second->renderer->GetDynamicSettings().instancesCount = visualObjects.size();
				ClearGameRenderers();

				for (const auto& vo : visualObjects)
				{
					vo.second->instanceDataTask = nullptr;
					vo.second->instanceIndex = std::nullopt;
				}

				const auto ib = std::make_shared<MMPEngine::Frontend::StructuredUploadBuffer<VisualObject::InstanceData>>(_globalContext, MMPEngine::Frontend::BaseStructuredBuffer::Settings{ visualObjects.size() });

				{
					const auto executor = _stream->CreateExecutor();
					_stream->Schedule(ib->CreateInitializationTask());
				}

				mr.second->instanceBuffer = ib;
				std::vector<std::shared_ptr<MMPEngine::Core::BaseTask>> resetTasks{};

				for (std::size_t i = 0; i < mr.second->renderer->GetDynamicSettings().instancesCount; ++i)
				{
					const auto resetTask = ib->CreateWriteStructTask(mr.second->GetDefaultInstanceData(), i);
					resetTasks.push_back(resetTask);
				}

				mr.second->resetInstanceBufferTask = std::make_shared<MMPEngine::Core::StaticBatchTask>(std::move(resetTasks));
			}

			//assign instances to visual objects
			_tmpFreeInstanceIndices.clear();

			for (std::size_t i = 0; i < mr.second->renderer->GetDynamicSettings().instancesCount; ++i)
			{
				_tmpFreeInstanceIndices.emplace(i);
			}

			for (const auto& vo : visualObjects)
			{
				if (vo.second->instanceIndex.has_value())
				{
					_tmpFreeInstanceIndices.erase(vo.second->instanceIndex.value());
				}
			}

			for (const auto& vo : visualObjects)
			{
				if (!vo.second->instanceIndex.has_value())
				{
					assert(!_tmpFreeInstanceIndices.empty());
					const auto idx = *(_tmpFreeInstanceIndices.cbegin());
					_tmpFreeInstanceIndices.erase(idx);

					vo.second->instanceDataTask = mr.second->instanceBuffer->CreateWriteStructTask(vo.second->instanceData, idx);
					vo.second->instanceIndex = idx;
				}
			}
			
		}
	}

	void RenderSystem::CreateGameRenderer()
	{
		const auto& meshRenderers = this->ObjectSystem<MeshRendererObject>::_objectsMap;
		const auto& visualObjects = this->ObjectSystem<VisualObject>::_objectsMap;
		const auto& gameRenderers = this->ObjectSystem<GameRendererObject>::_objectsMap;

		if (gameRenderers.empty() && !visualObjects.empty())
		{
			auto gameRenderer = std::make_unique<GameRendererObject>();

			const auto screen = std::make_shared<MMPEngine::Frontend::Screen>(_globalContext, MMPEngine::Core::Screen::Settings{
			0, 2, false, _config->GetCameraConfig().backgroundColor
				});

			const auto depthStencilTexture = std::make_shared<MMPEngine::Frontend::DepthStencilTargetTexture>(
				_globalContext,
				MMPEngine::Core::DepthStencilTargetTexture::Settings{
					MMPEngine::Core::DepthStencilTargetTexture::Settings::Format::Depth16,
						std::make_tuple(1.0f, 0),
					{MMPEngine::Core::TargetTexture::Settings::Antialiasing::MSAA_0, _globalContext->windowSize, "depth/stencil"}
				});

			{
				const auto executor = _stream->CreateExecutor();
				_stream->Schedule(screen->CreateInitializationTask());
				_stream->Schedule(depthStencilTexture->CreateInitializationTask());
			}

			const auto cameraNode = std::make_shared<MMPEngine::Core::Node>();
			cameraNode->localTransform.position = { 0.0f, 0.0f, -2.0f };

			MMPEngine::Core::OrthographicCamera::OrthographicSettings orthoSettings{};
			const auto fieldSize = _config->GetFieldConfig().size;

			orthoSettings.size = fieldSize;
			orthoSettings.size.x *= _config->GetCameraConfig().multiplier;
			orthoSettings.size.y *= _config->GetCameraConfig().multiplier;

			const auto camera = std::make_shared<MMPEngine::Frontend::OrthographicCamera>(
				_globalContext,
				MMPEngine::Core::OrthographicCamera::Settings{ orthoSettings, {1.0f, 3.0f} },
				cameraNode,
				MMPEngine::Core::Camera::Target{
					{ {screen->GetBackBuffer(), true} },
					{ depthStencilTexture, true }
				}
			);

			{
				const auto executor = _stream->CreateExecutor();
				_stream->Schedule(camera->CreateInitializationTask());
			}

			auto drawItems = std::vector<MMPEngine::Core::Camera::DrawCallsJob::Item>{};

			for (const auto& mr : meshRenderers)
			{
				if (mr.second->renderer->GetDynamicSettings().instancesCount == 0)
				{
					continue;
				}

				auto materialParams = MMPEngine::Core::BaseMaterial::Parameters{
					std::vector {
						MMPEngine::Core::BaseMaterial::Parameters::Entry {
							"camera_data",
								"scene",
								camera->GetUniformDataEntity(),
								MMPEngine::Core::BaseMaterial::Parameters::Buffer {
								MMPEngine::Core::BaseMaterial::Parameters::Buffer::Type::Uniform
							}
						},
						MMPEngine::Core::BaseMaterial::Parameters::Entry {
							"instance_data",
							"object",
								mr.second->instanceBuffer,
								MMPEngine::Core::BaseMaterial::Parameters::Buffer {
								MMPEngine::Core::BaseMaterial::Parameters::Buffer::Type::ReadonlyAccess
							}
						}
					}
				};

				auto matSettings = MMPEngine::Core::RenderingMaterial::Settings{};

				const auto material = std::make_shared<MMPEngine::Core::MeshMaterial>(
					matSettings,
					std::move(materialParams),
					mr.second->vertexShader,
					mr.second->pixelShader
				);

				drawItems.push_back(MMPEngine::Core::Camera::DrawCallsJob::Item{ mr.second->renderer, material });
			}


			const auto renderJob = std::make_shared<MMPEngine::Frontend::Camera::DrawCallsJob>(
				_globalContext,
				camera,
				std::move(drawItems)
			);

			{
				const auto executor = _stream->CreateExecutor();
				_stream->Schedule(camera->CreateTaskToUpdateUniformData());
				_stream->Schedule(renderJob->CreateInitializationTask());
			}

			const auto renderJobExecutionTask = renderJob->CreateExecutionTask();


			const auto screenFrameTask = screen->CreateFrameTask();
			screenFrameTask->GetTaskContext()->internalTask = std::make_shared<MMPEngine::Core::StaticBatchTask>(std::initializer_list<std::shared_ptr<MMPEngine::Core::BaseTask>>
			{
				renderJobExecutionTask
			});

			gameRenderer->rootTask = screenFrameTask;

			_world->AddObject(std::move(gameRenderer));
		}
	}

	void RenderSystem::Update(std::float_t dt)
	{
		CreateMeshRenderer();
		UpdateInstances();
		CreateGameRenderer();
	}

	void RenderSystem::OnViewportUpdated()
	{
		ClearGameRenderers();
	}

	void RenderSystem::OnRender()
	{
		const auto& gameRenderers = this->ObjectSystem<GameRendererObject>::_objectsMap;
		const auto& meshRenderers = this->ObjectSystem<MeshRendererObject>::_objectsMap;
		const auto& visualObjects = this->ObjectSystem<VisualObject>::_objectsMap;

		if (!gameRenderers.empty())
		{
			{
				const auto executor = _stream->CreateExecutor();

				for (const auto& mr : meshRenderers)
				{
					if (const auto resetInstanceBufferTask = mr.second->resetInstanceBufferTask)
					{
						_stream->Schedule(resetInstanceBufferTask);
					}
				}

				for (const auto& vo : visualObjects)
				{
					if (const auto it = vo.second->instanceDataTask)
					{
						_stream->Schedule(it);
					}
				}


				for (const auto& gr : gameRenderers)
				{
					_stream->Schedule(gr.second->rootTask);
				}
			}
		}
	}

	void RenderSystem::ClearGameRenderers()
	{
		const auto& gameRenderers = this->ObjectSystem<GameRendererObject>::_objectsMap;
		for (auto it = gameRenderers.begin(); it != gameRenderers.end();)
		{
			auto current = it;
			it = std::next(it);
			_world->RemoveObject(current->second->GetId());
		}
	}

}
