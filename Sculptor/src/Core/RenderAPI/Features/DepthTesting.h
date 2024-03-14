#pragma once

#include "Core/RenderAPI/Image/ImageView.h"
#include "Core/RenderAPI/Image/VulkanTexture.h"
#include "Core/RenderAPI/SwapChains/SwapChain.h"
#include "Core/RenderAPI/Utility/SupportUtility.h"

namespace Sculptor::Core
{
	class DepthTesting : public VulkanTexture, public ImageView
	{
	public:
		DepthTesting(std::weak_ptr<LogicalDevice> device, std::weak_ptr<SwapChain> swapChain) noexcept;

		void Create();

		VkImageView GetImageView() const;

		LOGICAL_DEVICE

		SWAP_CHAIN

		void CleanUp() const;

	protected:
		void Destroy() const override;

	private:
		VkImageView depthImageView;
	};
}
