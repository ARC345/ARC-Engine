#include "arc_pch.h"
#include "LayerStack.h"
#include "Layer.h"

namespace ARC {
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for(CLayer* layer : m_Layers) delete layer;
	}

	void LayerStack::PushLayer(CLayer* _layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, _layer);
		++m_LayerInsertIndex;
	}

	void LayerStack::PushOverlay(CLayer* _overlay)
	{
		m_Layers.emplace_back(_overlay);
	}

	void LayerStack::PopLayer(CLayer* _layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), _layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			--m_LayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(CLayer* _overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), _overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}