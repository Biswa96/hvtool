// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#pragma once

namespace hvtool {
    constexpr auto BUFF_SIZE = 0x1000;
    constexpr auto DEFAULT_PORT = 54321;

    namespace util {
        std::wstring from_bytes(const std::string &str);
        std::string to_bytes(const std::wstring &wstr);
        std::string sid_string();
        void create_pipe(const std::string &pipe_name);
    }
}
