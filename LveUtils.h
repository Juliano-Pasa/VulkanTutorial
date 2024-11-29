#pragma once

#include <functional>

namespace lve
{
	template <typename T, typename... Rest>
	void HashCombine(std::size_t& seed, const T& v, const Rest&... rest)
	{
		seed ^= std::hash<T>{}(v)+0x9e37779b9 + (seed << 6) + (seed >> 2);
		(HashCombine(seed, rest), ...);
	};
}