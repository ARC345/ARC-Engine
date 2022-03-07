//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------
#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\vector.h"
#include "imgui\imgui.h"
#include "ARC\Types\TybeBase.h"
#include "ARC\Events\KeyEvent.h"
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "ARC\Renderer\Color.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "ARC\Renderer\Texture.h"

//#define SpaceWar
#ifdef SpaceWar
	#include "SpaceWar\SpaceWar.h"
#else
	#include "Sandbox2D.h"
#endif

ARC::Core::CApplication* ARC::Core::CreateApplication()
{
	return new CSandbox;
}

//-------------------[Layer]----------------------//
CExampleLayer::CExampleLayer() : 
	ARC::CLayer("Example"),
	m_CameraController(1280.f / 780.f, true)
{
	//-------------------------------+[Code for triangle]+-------------------------------//
	{
		float triangle_verts[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		ARC::TRef<ARC::CVertexBuffer> triangle_vertex_buffer;
		triangle_vertex_buffer = ARC::CVertexBuffer::Create(triangle_verts, sizeof(triangle_verts));
		ARC::CBufferLayout triangle_layout = {
			{ ARC::EShaderDataType::Float3, "a_Position" },
			{ ARC::EShaderDataType::Float4, "a_Colour" }
		};
		triangle_vertex_buffer->SetLayout(triangle_layout);

		m_TriangleVertexArray = ARC::CVertexArray::Create();
		m_TriangleVertexArray->AddVertexBuffer(triangle_vertex_buffer);

		unsigned int triangle_indices[] = {
			0,1,2
		};

		ARC::TRef<ARC::CIndexBuffer> triangle_index_buffer;
		triangle_index_buffer = ARC::CIndexBuffer::Create(triangle_indices, sizeof(triangle_indices) / sizeof(uint32_t));
		m_TriangleVertexArray->SetIndexBuffer(triangle_index_buffer);

		//m_TriangleShader = ARC::CShader::Create();
	}
	//-------------------------------~[Code for triangle]~-------------------------------//
	//--------------------------------+[Code for square]+--------------------------------//
	m_SquareVertexArray = ARC::CVertexArray::Create();

	float square_verts[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	ARC::TRef<ARC::CVertexBuffer> square_vertex_buffer;
	square_vertex_buffer = ARC::CVertexBuffer::Create(square_verts, sizeof(square_verts));

	ARC::CBufferLayout square_layout = { 
		{ ARC::EShaderDataType::Float3, "a_Position" },
		{ ARC::EShaderDataType::Float2, "a_TexCoord" }
	};
	square_vertex_buffer->SetLayout(square_layout);

	m_SquareVertexArray->AddVertexBuffer(square_vertex_buffer);

	unsigned int square_indices[] = { 0, 1, 2, 2, 3, 0 };

	ARC::TRef<ARC::CIndexBuffer> square_index_buffer;
	square_index_buffer = ARC::CIndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
	m_SquareVertexArray->SetIndexBuffer(square_index_buffer);
	//--------------------------------~[Code for square]~--------------------------------//
	//---------------------------+[Code for FlatColor Shader]+---------------------------//
	const std::string flat_color_vertex_source = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
				}			
			)";
	const std::string flat_color_fragment_source = R"(
				#version 330 core

				layout(location = 0) out vec4 o_Color;

				in vec3 v_Position;
				
				uniform vec4 u_Color;

				void main()
				{
					o_Color = u_Color;
				}			
			)";
	m_FlatColorShader = ARC::CShader::Create("FlatColorShader", flat_color_vertex_source, flat_color_fragment_source);

	//---------------------------~[Code for FlatColor Shader]~---------------------------//
	//----------------------------+[Code for Texture Shader]+----------------------------//
	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	//----------------------------~[Code for Texture Shader]~----------------------------//

	m_Texture = ARC::CTexture2D::Create("assets/textures/Checkerboard.png");
	m_TestTexture = ARC::CTexture2D::Create("assets/textures/ChernoLogo.png");

	textureShader->Bind();
	textureShader->Set<int>("u_Texture", 0);
}

void CExampleLayer::OnUpdate(float _DeltaTime)
{
	m_CameraController.OnUpdate(_DeltaTime);

	ARC::CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
	ARC::CRenderCommand::Clear();

	ARC::CRenderer::BeginScene(m_CameraController.GetCamera());
	//SQ_Data.Scale = {0.1f, 0.1f, 0.1f};

	m_FlatColorShader->Bind();
	m_FlatColorShader->Set<ARC::CColor>("u_Color", SQ_Colour);

	//for (size_t x = 0; x < 20; x++)
	//	for (size_t y = 0; y < 20; y++)
	//	{
	//		glm::vec3 pos = { x * 0.11f, y * 0.11f, 0.f };
	//		pos += SQ_Data.Position;
	//		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), pos), SQ_Data.Scale);
	//		ARC::CRenderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
	//	}

	m_Texture->Bind();
	ARC::CRenderer::Submit(m_ShaderLibrary.GetShader("Texture"), m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_TestTexture->Bind();
	ARC::CRenderer::Submit(m_ShaderLibrary.GetShader("Texture"), m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//ARC::CRenderer::Submit(m_TriangleShader, m_TriangleVertexArray);
	ARC::CRenderer::EndScene();

	GetCam().RecalculateViewProjectionMatrix();
}

void CExampleLayer::OnEvent(ARC::CEvent& _Event)
{
	m_CameraController.OnEvent(_Event);
}

bool CExampleLayer::OnKeyPressedEvent(ARC::CKeyPressedEvent& _Event)
{
	return false;
}

void CExampleLayer::OnGuiRender()
{
	ImGui::Begin("Sandbox");
	if (ImGui::TreeNode("Camera"))
 	{
// 		ImGui::DragFloat3("CamLocation", &GetCam().Position[0]);
// 		ImGui::DragFloat("CamMoveSpeed", &CamMoveSpeed);
// 		ImGui::DragFloat("CamRotation", &GetCam().Rotation, 1.0f, 0, 360);
// 		ImGui::DragFloat("CamRotSpeed", &CamRotSpeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	ImGui::End();
}

//-------------------[Layer]----------------------//
CSandbox::CSandbox()
{
	#ifdef SpaceWar
		PushLayer(new CSpaceWar());
	#else
		PushLayer(new CSandbox2D());
	#endif
}
