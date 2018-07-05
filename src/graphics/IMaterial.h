#pragma once

namespace Nova {
	class IMaterial :public Bindable, public ResourceBase
	{
	public:
		virtual ~IMaterial() = default;		
		//virtual std::shared_ptr<IGPUProgram> getGPUProgram() const = 0;
	};
}