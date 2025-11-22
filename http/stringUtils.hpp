#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

std::string normalizePolishChars(const std::string& text);
void getWords(const std::string& token, std::vector<std::string>& results);
std::string wordToLower(const std::string& str);

//