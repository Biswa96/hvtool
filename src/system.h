// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#pragma once

namespace hvtool {
    namespace system {
        std::string get_vm_id(const std::string &vm_name);
        void create_vm(const std::string &json_file);
    }
}
