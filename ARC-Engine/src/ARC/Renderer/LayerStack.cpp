#include "arc_pch.h"
#include "layer_stack.h"
#include "Layer.h"

namespace arc {
	layer_stack::layer_stack()
	{
	}

	layer_stack::~layer_stack()
	{
		for(layer* layer : layers_){
			layer->on_detach();
			delete layer;
		}
	}

	void layer_stack::push_layer(layer* _layer)
	{
		layers_.emplace(layers_.begin() + layer_insert_index_, _layer);
		++layer_insert_index_;
	}

	void layer_stack::push_overlay(layer* _overlay)
	{
		layers_.emplace_back(_overlay);
	}

	void layer_stack::PopLayer(layer* _layer)
	{
		auto it = std::find(layers_.begin(), layers_.end(), _layer);
		if (it != layers_.end()) {
			layers_.erase(it);
			--layer_insert_index_;
		}
	}

	void layer_stack::PopOverlay(layer* _overlay)
	{
		auto it = std::find(layers_.begin(), layers_.end(), _overlay);
		if (it != layers_.end()) {
			layers_.erase(it);
		}
	}
}