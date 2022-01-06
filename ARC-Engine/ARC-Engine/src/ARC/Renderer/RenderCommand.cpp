#include "arc_pch.h"
#include "RenderCommand.h"

#include "Platform/OpenGl/OpenGLRendererAPI.h"

namespace ARC {
	CRendererAPI* CRenderCommand::s_RendererAPI = new COpenGLRendererAPI; 
}