#pragma once

namespace Nova {
	class ISubSystem
	{
	public:
		virtual void startUp() = 0;
		virtual void shutDown() = 0;
	};
}
