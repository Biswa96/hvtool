// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#include <windows.h>
#include <iostream>

#include "system.h"

int main() {
    try {
        hvtool::system::create_vm("hvtool.json");
    } catch (std::exception &e) {
        std::cout << e.what() << "\n";
    }
}
