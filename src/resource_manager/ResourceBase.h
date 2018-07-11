#pragma once
#include <string>

namespace Nova {
	class ResourceBase
	{
	public:
		ResourceBase() = default;
		virtual ~ResourceBase() = default;

		const std::string& getResourceName() const { return mResourceName; }
		//bool isLoaded() const { return mResourcedLoaded; };
		//virtual uint32_t getResourceID() = 0;
		//const type_info& getType() { return typeid(*this); };
	protected:		
		const std::string mResourceName;
		//bool mResourcedLoaded = false;
	};

}