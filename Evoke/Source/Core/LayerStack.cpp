#include "PCH.h"
#include "LayerStack.h"

namespace Evoke
{

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			layer->Detached();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* inLayer)
	{
		mLayers.emplace(begin() + mLayerInsertIndex, inLayer);
		mLayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* inLayer)
	{
		mLayers.emplace_back(inLayer);
	}

	void LayerStack::RemoveLayer(Layer* inLayer)
	{
		auto it = std::find(begin(), begin() + mLayerInsertIndex, inLayer);
		if (it != begin() + mLayerInsertIndex)
		{
			inLayer->Detached();
			mLayers.erase(it);
			mLayerInsertIndex--;
		}
	}

	void LayerStack::RemoveOverlay(Layer* inLayer)
	{
		auto it = std::find(begin() + mLayerInsertIndex, end(), inLayer);
		if (it != end())
		{
			inLayer->Detached();
			mLayers.erase(it);
		}
	}

}
