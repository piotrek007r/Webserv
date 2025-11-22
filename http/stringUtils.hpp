#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

std::string normalizePolishChars(const std::string& text);
void getWords(const std::string& inputToSplit, std::vector<std::string>& results, std::string words);
std::string wordToLower(const std::string& str);

//