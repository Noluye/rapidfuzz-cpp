#include <iostream>
#include <rapidfuzz/fuzz.hpp>
#include <string>
#include <optional>

#include <Windows.h>

template <typename Sentence1, typename Iterable, typename Sentence2 = typename Iterable::value_type>
std::vector<std::pair<Sentence2, double>> extract(const Sentence1& query, const Iterable& choices,
												  const double score_cutoff = 0.0)
{
	std::vector<std::pair<Sentence2, double>> results;

	//rapidfuzz::fuzz::CachedRatio<typename Sentence1::value_type> scorer(query);
	rapidfuzz::fuzz::CachedPartialRatio<typename Sentence1::value_type> scorer(query);

	for (const auto& choice : choices) {
		double score = scorer.similarity(choice, score_cutoff);

		if (score >= score_cutoff) {
			results.emplace_back(choice, score);
		}
	}
	return results;
}

template <typename Sentence1,
	typename Iterable, typename Sentence2 = typename Iterable::value_type>
std::optional<std::pair<Sentence2, double>>
extractOne(const Sentence1& query, const Iterable& choices, const double score_cutoff = 0.0)
{
	bool match_found = false;
	double best_score = score_cutoff;
	Sentence2 best_match;

	rapidfuzz::fuzz::CachedRatio<typename Sentence1::value_type> scorer(query);

	for (const auto& choice : choices) {
		double score = scorer.similarity(choice, best_score);

		if (score >= best_score) {
			match_found = true;
			best_score = score;
			best_match = choice;
		}
	}

	if (!match_found) {
		return std::nullopt;
	}

	return std::make_pair(best_match, best_score);
}

void Test1()
{
	std::string a = "hello world";
	std::string b = "hello world!";
	std::cout << rapidfuzz::fuzz::ratio(a, b) << std::endl;
	std::cout << rapidfuzz::fuzz::partial_ratio(a, b) << std::endl;
}


//将wstring转换成string  
std::string wstring2string(std::wstring wstr)
{
	std::string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

void Test2()
{
	std::cout << "===================================================" << std::endl;
	std::wstring query = L"今天天气真好";
	std::vector<std::wstring> choices = {L"天气", L"你好", L"介绍自己"};

	auto answers = extract(query, choices);
	for (auto& answer : answers)
	{
		std::cout << wstring2string(answer.first) << ": " << answer.second << std::endl;
	}
}
void Test3()
{
	std::cout << "===================================================" << std::endl;
	std::string query = "今天天气真好";
	std::vector<std::string> choices = { "天气", "你好", "介绍自己" };

	auto answers = extract(query, choices);
	for (auto& answer : answers)
	{
		std::cout << answer.first << ": " << answer.second << std::endl;
	}
}

void Test4()
{
	std::cout << "===================================================" << std::endl;
	std::string query = "今天天气真好";
	std::vector<std::string> choices = { "天气", "你好", "介绍自己" };

	auto answers = extractOne(query, choices);
	auto ans = answers.value_or(std::make_pair("empty", 0));
	std::cout << ans.first << ": " << ans.second << std::endl;
}

int main()
{
	std::string a = "aaaa";
	std::string b = "abab";
	std::cout << rapidfuzz::fuzz::ratio(a, b) << std::endl;

	Test1();
	Test2();
	Test3();
	Test4();
}