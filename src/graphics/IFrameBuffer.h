#pragma once

namespace Nova {
	class IFrameBuffer :public Bindable, public ResourceBase
	{
	public:
		IFrameBuffer() = default;
		virtual ~IFrameBuffer() = default;
	};

}