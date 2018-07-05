#pragma once

namespace Nova {
	class IInputSystem
	{
	public:
		IInputSystem() = default;
		virtual ~IInputSystem() = default;
		virtual void processInputs() = 0;
	protected:
	};	
}
