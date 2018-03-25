#include "Renderer.hpp"

template<typename Func>
Func Renderer::GetExtensionFunc(const std::string& name)
{
	return (Func)vkGetInstanceProcAddr(instance, name.c_str());
}
