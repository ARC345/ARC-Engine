#include "arc_pch.h"
#include "LayerStack.h"
#include "Layer.h"

namespace ARC {
	LayerStack::LayerStack()
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for(Layer* layer : m_Layers) delete layer;
	}

	void LayerStack::PushLayer(Layer* _layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, _layer);
	}

	void LayerStack::PushOverlay(Layer* _overlay)
	{
		m_Layers.emplace_back(_overlay);
	}

	void LayerStack::PopLayer(Layer* _layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), _layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* _overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), _overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}