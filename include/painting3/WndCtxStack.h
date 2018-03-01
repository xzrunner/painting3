#pragma once

#include "painting3/WindowContext.h"

#include <cu/cu_macro.h>
#include <cu/cu_stl.h>

namespace pt3
{

class WndCtxStack
{
public:
	int  Push(const WindowContext& ctx);
	void Pop();

	const WindowContext* Top() const;

	int Size() const { return m_stack.size(); }

	// todo
	bool Bind(int idx);
	
private:
	static void BindCtx(const WindowContext& ctx);

private:
	CU_VEC<WindowContext> m_stack;

	CU_SINGLETON_DECLARATION(WndCtxStack)

}; // WndCtxStack

}