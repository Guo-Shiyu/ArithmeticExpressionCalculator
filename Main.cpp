#include <iostream>
#include <filesystem>
#include <fstream>

#include "State.h"

namespace fs = std::filesystem;
using namespace aec;

/*
auto read_to_string = [](const std::string& file) -> std::optional<std::string>
{
	std::ifstream in{ file, std::ios::in };
	if (in.is_open())
		return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	else
		return std::nullopt;
};
*/

auto read_to_vec(const std::string &file) -> std::optional<std::vector<std::string>>
{
	std::ifstream in{file, std::ios::in};
	if (in.is_open())
	{
		std::vector<std::string> result;
		while (in.good())
		{
			std::string line;
			std::getline(in, line);
			if (line.size() > 0 and not(line.starts_with(' ') || line.starts_with('\n')))
				result.push_back(std::move(line));
		}
		if (result.size() == 0)
			return std::nullopt;
		else
			return result;
	}
	else
		return std::nullopt;
}

void integration_test(const fs::path &test_dictory_path)
{
	auto &path = test_dictory_path;
	auto pathname = path.string();
	auto begin = fs::directory_iterator{path}, end = fs::directory_iterator{};
	while (begin != end)
	{
		auto filename = begin->path().filename().string();
		auto whole_path = path.string().append("\\").append(filename);
		auto texts = read_to_vec(whole_path);
		if (not texts.has_value())
			std::cout << "test file in path: " << whole_path << " open failed" << std::endl;
		else
		{
			auto exprs = texts.value();
			std::cout << "file: " << filename << " :\n" 
				<< "=================" << std::endl;

			for (auto &expr : exprs)
			{
				std::cout << "raw expr:" << '|' << expr << '|' << std::endl;
				try
				{
					State::load_expr(std::move(expr))

						/// HelloPass defined in Operations.h, which is an empty struct
						/// uncomment this block to have a try

						// .register_pass<HelloPass>([](auto _)
						// 						  { std::cout << "Before Hello Pass Execute" << std::endl; },
						// 						  [](auto _)
						// 						  {
						// 							  std::cout << "After Hello Pass Execute" << std::endl;
						// 						  })

						.register_pass<Printer>([](auto _)
												{ std::cout << "Dump:"; },
												[](auto &printer)
												{
													std::cout << '|' << printer.result() << '|' << std::endl;
												})
						.register_pass<Evaluator>([](auto _)
												  { std::cout << "Eval:"; },
												  [](auto &evaluator)
												  {
													  std::cout << evaluator.result() << std::endl;
												  })

						//.register_pass<Prosthaphaeresis>(std::nullopt, std::nullopt)
						//.register_pass<Printer>([](auto _)
						//	{
						//		std::cout << "Dump:";
						//	},
						//	[](auto& printer)
						//	{
						//		std::cout << '|' << printer.result() << '|' << std::endl;
						//	})

						.start();
				}
				catch (const std::string &err_info)
				{
					std::cout << "error:" << err_info << std::endl;
				}
				catch (const std::exception& e)
				{
					std::cout << "error:" << e.what() << std::endl;
				}

				std::cout << std::endl;
			}
		}
		++begin;
	}
}

int main(int argc, char **argv)
{
	/*if (argc != 2)
	{
		std::cout << "Usage:" << argv[0] << " <arithmetic-expression>" << std::endl;
		std::exit(0);
	}*/

	auto cur_path = fs::current_path();
	// std::cout << "current path: " << cur_path.string() << std::endl;

	auto test_path = cur_path.append("test");
	if (not fs::exists(test_path))
		std::cout << "test file path " << test_path.string() << " not exists" << std::endl;
	else
		integration_test(test_path);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
