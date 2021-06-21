#include "Object.h"
#include "..\include\Object.h"

void Object::Render()
{

	m_Model->Render(*m_Shader);
}
