#pragma once

#include <functional>

#include "Ast.h"
#include "Operations.h"

namespace aec
{
	using std::function;

	template <typename T>
	concept ValidPass = requires(T *pass_ptr)
	{
		{static_cast<PassTrait *>(pass_ptr)};
	};

	class State
	{
	public:
		State(const State &) = default;
		~State();

	private:
		State() = delete;
		State(AbstractSyntaxTree &&tree);

	public:
		static State load_expr(string &&text);

	public:
		template <ValidPass P>
		State &register_pass(optional<typename BasePass<P>::HookFn> &&pre, optional<typename BasePass<P>::HookFn> &&after)
		{
			auto pass_ptr = new P();
			pass_ptr->before = pre;
			pass_ptr->after = after;
			_pass.push_back(pass_ptr);
			return *this;
		}

		State &start();

	private:
		AbstractSyntaxTree _ast;
		vector<PassTrait *> _pass;
	};

}