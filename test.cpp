/**
 * Simple ponci test application.
 *
 * Compile with:
 *   g++ -std=c++11 -o ponic_test ponci.cpp test.cpp -lpthread
 *
 * Copyright 2016 by LRR-TUM
 * Jens Breitbart     <j.breitbart@tum.de>
 *
 * Licensed under GNU General Public License 2.0 or later.
 * Some rights reserved. See LICENSE
 */

#include <iostream>
#include <thread>
#include <string>

#include <unistd.h>

#include "ponci.hpp"

static void sleeper(const std::string name) {
	cgroup_create(name.c_str());
	size_t arr[] = {0};

	cgroup_set_cpus(name.c_str(), arr, 1);
	cgroup_set_mems(name.c_str(), arr, 1);
	cgroup_add_me(name.c_str());

	std::cout << "Going to freeze me. brrrr" << std::endl;
	std::cout << "Please press return to thaw me." << std::endl;
	cgroup_freeze(name.c_str());

	std::cout << "Aaaah, its warm again!" << std::endl;
}

int main(int argc, char const *argv[]) {
	const std::string name("ponci_test1");

	cgroup_create(name.c_str());

	size_t arr[] = {0, 2, 3};

	cgroup_set_cpus(name.c_str(), arr, 3);
	cgroup_set_mems(name.c_str(), arr, 1);
	cgroup_add_me(name.c_str());

	cgroup_set_memory_migrate(name.c_str(), 1);
	cgroup_set_cpus_exclusive(name.c_str(), 1);
	cgroup_set_mem_hardwall(name.c_str(), 1);
	cgroup_set_scheduling_domain(name.c_str(), -1);

	std::string sleeper_name = name + std::string("/ponci_sleeper");
	std::thread t_freeze(sleeper, sleeper_name);

	// wait for user input
	std::cin.ignore();

	cgroup_thaw(sleeper_name.c_str());

	t_freeze.join();

	cgroup_delete(sleeper_name.c_str());

	return 0;
}