#pragma once

#include <cu/cu_macro.h>

#include <memory>

namespace pt3
{

class WindowContext;

class Blackboard
{
public:
	//void SetWindowContext(const std::shared_ptr<WindowContext>& wc) { m_wc = wc; }
	//const std::shared_ptr<WindowContext>& GetWindowContext() { return m_wc; }

private:
	std::shared_ptr<WindowContext> m_wc = nullptr;

	CU_SINGLETON_DECLARATION(Blackboard);

}; // Blackboard

}