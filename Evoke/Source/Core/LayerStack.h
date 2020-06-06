#pragma once
#include "PCH.h"
#include "Core.h"
#include "Layer.h"

namespace Evoke
{
	class LayerStack
	{
	public:
		using Container = std::vector<Layer*>;

		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* inLayer);
		void PushOverlay(Layer* inLayer);
		void RemoveLayer(Layer* inLayer);
		void RemoveOverlay(Layer* inLayer);

		Container::iterator begin() { return mLayers.begin(); }
		Container::iterator end() { return mLayers.end(); }
		Container::reverse_iterator rbegin() { return mLayers.rbegin(); }
		Container::reverse_iterator rend() { return mLayers.rend(); }

		Container::const_iterator begin() const { return mLayers.begin(); }
		Container::const_iterator end() const { return mLayers.end(); }
		Container::const_reverse_iterator rbegin() const { return mLayers.rbegin(); }
		Container::const_reverse_iterator rend() const { return mLayers.rend(); }
	private:
		Container mLayers;
		u32 mLayerInsertIndex = 0;
	};
}