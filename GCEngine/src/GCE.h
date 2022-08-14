#pragma once

//For use by GCE applications
//CORE---------------------------------------------
#include "GCE/Core/Application.h"
#include "GCE/Core/Layer.h"
#include "GCE/Core/Log.h"
#include "GCE/Core/Assert.h"

#include "GCE/Core/Timestep.h"

#include "GCE/Core/Input.h"
#include "GCE/Core/KeyCodes.h"
#include "GCE/Core/MouseButtonCodes.h"
#include "GCE/Renderer/OrthographicCameraController.h"

#include "GCE/Scene/Scene.h"
#include "GCE/Scene/Entity.h"
#include "GCE/Scene/ScriptableEntity.h"
#include "GCE/Scene/Components.h"
//--------------------------------------------------



//IMGUI---------------------------------------------
#include "GCE/ImGui/ImGuiLayer.h"
//--------------------------------------------------



//RENDERER------------------------------------------
#include "GCE/Renderer/Renderer.h"
#include "GCE/Renderer/Renderer2D.h"
#include "GCE/Renderer/RenderCommand.h"

#include "GCE/Renderer/Buffer.h"
#include "GCE/Renderer/Shader.h"
#include "GCE/Renderer/FrameBuffer.h"
#include "GCE/Renderer/Texture.h"
#include "GCE/Renderer/SubTexture2D.h"
#include "GCE/Renderer/VertexArray.h"

#include "GCE/Renderer/OrthographicCamera.h"
//--------------------------------------------------