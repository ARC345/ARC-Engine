#pragma once
#include <vector>
#include "Layer.h"

namespace ARC {
	class ARC_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(CLayer* _layer);
		void PushOverlay(CLayer* _overlay);
		void PopLayer(CLayer* _layer);
		void PopOverlay(CLayer* _overlay);

		std::vector<CLayer*>::iterator begin() { return mLayers.begin(); }
		std::vector<CLayer*>::iterator end() { return mLayers.end(); }
	private:
		std::vector<CLayer*> mLayers;
		unsigned int mLayerInsertIndex=0;
	};
}