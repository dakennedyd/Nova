#pragma once

namespace Nova {
	class ITexture :public Bindable, public ResourceBase
	{
	public:		
		virtual ~ITexture() = default;
		
	};
}
