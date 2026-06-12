#pragma once
#include <vector>
#include "Layer.h"

namespace arc {
	class ARC_API layer_stack
	{
	public:
		layer_stack();
		~layer_stack();

		void push_layer(layer* _layer);
		void push_overlay(layer* _overlay);
		void PopLayer(layer* _layer);
		void PopOverlay(layer* _overlay);

		std::vector<layer*>::iterator begin() { return layers_.begin(); }
		std::vector<layer*>::iterator end() { return layers_.end(); }
	private:
		std::vector<layer*> layers_;
		unsigned int layer_insert_index_=0;
	};
}