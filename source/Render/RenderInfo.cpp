#include <cassert>
#include "RenderInfo.hpp"


auto RenderInfo::GetStack() -> std::stack<RenderInfo> &
{
	static std::stack<RenderInfo> stack;
	return stack;
}

void RenderInfo::Push( const RenderInfo& settings )
{
	GetStack().push( settings );
}

auto RenderInfo::Pop() -> RenderInfo
{
	RenderInfo result;

	auto& stack = GetStack();

	assert( !stack.empty() );
	if ( !stack.empty() ) {
		result = stack.top();
		stack.pop();
	}

	return result;
}

auto RenderInfo::Top() -> RenderInfo&
{
	auto& stack = GetStack();

	if ( stack.empty() )
	{
		assert( false );
		static RenderInfo settings;
		return settings;
	}

	return stack.top();
}

void RenderInfo::Clear()
{
	auto& stack = GetStack();
	while ( !stack.empty() ) {
		stack.pop();
	}
}
