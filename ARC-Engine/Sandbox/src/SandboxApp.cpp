#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\vector.h"
#include "imgui\imgui.h"
#include "ARC\Types\TybeBase.h"
#include "ARC\Helpers\Macros.h"
#include "ARC\Events\KeyEvent.h"
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "ARC\Renderer\Color.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "ARC\Renderer\Texture.h"

ARC::Core::CApplication* ARC::Core::CreateApplication()
{
	return new CSandbox;
}

//-------------------[Layer]----------------------//
CExampleLayer::CExampleLayer() : 
	ARC::CLayer("Example"),
	m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
	//-------------------------------+[Code for triangle]+-------------------------------//
	{
		float triangle_verts[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		ARC::TRef<ARC::CVertexBuffer> triangle_vertex_buffer;
		triangle_vertex_buffer.reset(ARC::CVertexBuffer::Create(triangle_verts, sizeof(triangle_verts)));
		ARC::CBufferLayout triangle_layout = {
			{ ARC::EShaderDataType::Float3, "a_Position" },
			{ ARC::EShaderDataType::Float4, "a_Colour" }
		};
		triangle_vertex_buffer->SetLayout(triangle_layout);

		m_TriangleVertexArray.reset(ARC::CVertexArray::Create());
		m_TriangleVertexArray->AddVertexBuffer(triangle_vertex_buffer);

		unsigned int triangle_indices[] = {
			0,1,2
		};

		ARC::TRef<ARC::CIndexBuffer> triangle_index_buffer;
		triangle_index_buffer.reset(ARC::CIndexBuffer::Create(triangle_indices, sizeof(triangle_indices) / sizeof(uint32_t)));
		m_TriangleVertexArray->SetIndexBuffer(triangle_index_buffer);

		const std::string triangle_vertex_source = R"(
					#version 330 core

					layout(location = 0) in vec3 a_Position;
					layout(location = 1) in vec4 a_Colour;

					uniform mat4 u_ViewProjection;
					uniform mat4 u_Transform;
					
					out vec3 v_Position;
					out vec4 v_Colour;

					void main()
					{
						v_Position = a_Position;
						v_Colour = a_Colour;
						gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
					}			
				)";
		const std::string triangle_fragment_source = R"(
					#version 330 core

					layout(location = 0) out vec4 o_Colour;
					in vec3 v_Position;
					in vec4 v_Colour;
					void main()
					{
						o_Colour = vec4((v_Position+1)*0.5, 1.0);
						o_Colour = v_Colour;
					}			
				)";
		m_TriangleShader.reset(ARC::CShader::Create(triangle_vertex_source, triangle_fragment_source));

	}
	//-------------------------------~[Code for triangle]~-------------------------------//
	//--------------------------------+[Code for square]+--------------------------------//
	m_SquareVertexArray.reset(ARC::CVertexArray::Create());

	float square_verts[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	ARC::TRef<ARC::CVertexBuffer> square_vertex_buffer;
	square_vertex_buffer.reset(ARC::CVertexBuffer::Create(square_verts, sizeof(square_verts)));

	ARC::CBufferLayout square_layout = { 
		{ ARC::EShaderDataType::Float3, "a_Position" },
		{ ARC::EShaderDataType::Float2, "a_TexCoord" }
	};
	square_vertex_buffer->SetLayout(square_layout);

	m_SquareVertexArray->AddVertexBuffer(square_vertex_buffer);

	unsigned int square_indices[] = { 0, 1, 2, 2, 3, 0 };

	ARC::TRef<ARC::CIndexBuffer> square_index_buffer;
	square_index_buffer.reset(ARC::CIndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t)));
	m_SquareVertexArray->SetIndexBuffer(square_index_buffer);

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

	m_FlatColorShader.reset(ARC::CShader::Create(flat_color_vertex_source, flat_color_fragment_source));
	//--------------------------------~[Code for square]~--------------------------------//
	//----------------------------+[Code for Texture Shader]+----------------------------//
	const std::string texture_vertex_source = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec2 a_TexCoord;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec2 v_TexCoord;

				void main()
				{
					v_TexCoord = a_TexCoord;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
				}			
			)";
	const std::string texture_fragment_source = R"(
				#version 330 core

				layout(location = 0) out vec4 o_Color;

				uniform sampler2D u_Texture;

				in vec2 v_TexCoord;

				void main()
				{
					o_Color = texture(u_Texture, v_TexCoord);
				}			
			)";

	m_TextureShader.reset(ARC::CShader::Create(texture_vertex_source, texture_fragment_source));
	//----------------------------~[Code for Texture Shader]~----------------------------//

	m_Texture = ARC::CTexture2D::Create("assets/textures/Checkerboard.png");
	std::dynamic_pointer_cast<ARC::COpenGLShader>(m_TextureShader)->Bind();
	std::dynamic_pointer_cast<ARC::COpenGLShader>(m_TextureShader)->UploadUniform<int>("u_Texture", 0);
}

void CExampleLayer::OnUpdate(float _DeltaTime)
{
	if (ARC::CInput::IsKeyPressed(ARC_KEY_LEFT))
		m_Camera.Position.x -= CamMoveSpeed*_DeltaTime;
	if (ARC::CInput::IsKeyPressed(ARC_KEY_RIGHT))
		m_Camera.Position.x += CamMoveSpeed*_DeltaTime;

	if (ARC::CInput::IsKeyPressed(ARC_KEY_UP))
		m_Camera.Position.y += CamMoveSpeed*_DeltaTime;
	if (ARC::CInput::IsKeyPressed(ARC_KEY_DOWN))
		m_Camera.Position.y -= CamMoveSpeed*_DeltaTime;

	if (ARC::CInput::IsKeyPressed(ARC_KEY_J))
		m_Camera.Rotation += CamRotSpeed*_DeltaTime;
	if (ARC::CInput::IsKeyPressed(ARC_KEY_L))
		m_Camera.Rotation -= CamRotSpeed*_DeltaTime;

	if (ARC::CInput::IsKeyPressed(ARC_KEY_A))
		SQ_Data.Position.x -= SQ_MoveSpeed * _DeltaTime;
	if (ARC::CInput::IsKeyPressed(ARC_KEY_D))
		SQ_Data.Position.x += SQ_MoveSpeed * _DeltaTime;
		
	if (ARC::CInput::IsKeyPressed(ARC_KEY_W))
		SQ_Data.Position.y += SQ_MoveSpeed * _DeltaTime;
	if (ARC::CInput::IsKeyPressed(ARC_KEY_S))
		SQ_Data.Position.y -= SQ_MoveSpeed * _DeltaTime;

	ARC::CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
	ARC::CRenderCommand::Clear();

	ARC::CRenderer::BeginScene(m_Camera);
	SQ_Data.Scale = {0.1f, 0.1f, 0.1f};

	std::dynamic_pointer_cast<ARC::COpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<ARC::COpenGLShader>(m_FlatColorShader)->UploadUniform<glm::vec4>("u_Color", SQ_Colour);

	for (size_t x = 0; x < 20; x++)
		for (size_t y = 0; y < 20; y++)
		{
			glm::vec3 pos = { x * 0.11f, y * 0.11f, 0.f };
			pos += SQ_Data.Position;
			glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), pos), SQ_Data.Scale);
			ARC::CRenderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
		}

	m_Texture->Bind();
	ARC::CRenderer::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	//ARC::CRenderer::Submit(m_TriangleShader, m_TriangleVertexArray);
	ARC::CRenderer::EndScene();

	GetCam().RecalculateViewProjectionMatrix();
}

void CExampleLayer::OnEvent(ARC::CEvent& _Event)
{
	ARC::CEventDispatcher dispatcher(_Event);
	dispatcher.Dispatch<ARC::CKeyPressedEvent>(BIND_FN(&CExampleLayer::OnKeyPressedEvent));
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
		ImGui::DragFloat3("CamLocation", &GetCam().Position[0]);
		ImGui::DragFloat("CamMoveSpeed", &CamMoveSpeed);
		ImGui::DragFloat("CamRotation", &GetCam().Rotation, 1.0f, 0, 360);
		ImGui::DragFloat("CamRotSpeed", &CamRotSpeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", glm::value_ptr(SQ_Colour));
		ImGui::TreePop();
	}
	ImGui::End();
}

//-------------------[Layer]----------------------//
CSandbox::CSandbox()
{
	PushLayer(new CExampleLayer());
}
