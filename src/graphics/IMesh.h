#pragma once
#include "resource_manager/ResourceBase.h"

namespace Nova {
	class IMesh :public Bindable, public ResourceBase
	{
	public:		
		virtual ~IMesh() = default;		
		/*virtual const int getNumVertices() const = 0;
		virtual const int getNumIndices() const = 0;
		virtual const int getMode() const = 0;			//what mode the mesh is in triangles, lines, points etc
		virtual const Mat4& getModelMatrix() const = 0;
		virtual void setModelMatrix(Mat4 model) = 0;*/
		//virtual void draw() const = 0;
	protected:	
		
	};
}
