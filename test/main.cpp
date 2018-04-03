#include "common.h"

#include "external_struct.h"
#include "basic_object.h"
#include "adv_object.h"

int main(int argc, char const *argv[])
{
	craft::types::boot();

	auto ret = bandit::run(argc, (char**)argv);

	std::cin.get();
	return ret;
}
