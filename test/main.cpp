#include "types/common.h"
#include "types/core.h"

#include <spdlog/spdlog.h>
#include <bandit/bandit.h>

int main(int argc, char const *argv[])
{
	craft::types::boot();

	auto ret = bandit::run(argc, (char**)argv);

	std::cin.get();
	return ret;
}
