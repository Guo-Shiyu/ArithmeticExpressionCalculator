#include "State.h"

using namespace aec;

aec::State::State(AbstractSyntaxTree &&tree) : _ast(std::forward<AbstractSyntaxTree>(tree)) {}

State aec::State::load_expr(string &&text)
{
	return State(AbstractSyntaxTree::parse(std::forward<string>(text)));
}

State &aec::State::start()
{
	for (auto &pass : _pass)
		pass->execute(_ast);
	return *this;
}

aec::State::~State()
{
	for (auto pass_ptr : _pass)
		delete pass_ptr;
}