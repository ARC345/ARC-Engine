#include "arc_pch.h"
#include "LayerStack.h"
#include "Layer.h"

namespace ARC {
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for(CLayer* layer : mLayers){
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(CLayer* _layer)
	{
		mLayers.emplace(mLayers.begin() + mLayerInsertIndex, _layer);
		++mLayerInsertIndex;
	}

	void LayerStack::PushOverlay(CLayer* _overlay)
	{
		mLayers.emplace_back(_overlay);
	}

	void LayerStack::PopLayer(CLayer* _layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _layer);
		if (it != mLayers.end()) {
			mLayers.erase(it);
			--mLayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(CLayer* _overlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), _overlay);
		if (it != mLayers.end()) {
			mLayers.erase(it);
		}
	}
}