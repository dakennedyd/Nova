#pragma once

namespace Nova {
	class Bindable
	{
	public:		
		virtual ~Bindable() = default;

		virtual void bind() const = 0;
		virtual void unBind() const = 0;
	};
}

