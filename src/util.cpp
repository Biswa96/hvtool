// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#include <windows.h>
#include <sddl.h>
#include <array>
#include <iostream>
#include <string>

#include "wil/resource.h"
#include "wil/token_helpers.h"

#include "util.h"

std::wstring hvtool::util::from_bytes(const std::string &str) {
    int len = 0;
    std::wstring wstr;

    THROW_LAST_ERROR_IF((len = MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int)str.length(), nullptr, 0)) == 0);
    wstr.resize(len);
    THROW_LAST_ERROR_IF(!MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], len));
    return wstr;
}

std::string hvtool::util::to_bytes(const std::wstring &wstr) {
    int len = 0;
    std::string str;

    THROW_LAST_ERROR_IF((len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
        (int)wstr.length(), nullptr, 0, nullptr, nullptr)) == 0);
    str.resize(len);
    THROW_LAST_ERROR_IF(!WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
        (int)wstr.length(), &str[0], len, nullptr, nullptr));
    return str;
}

std::string hvtool::util::sid_string() {
    std::string sec_desc;
    wil::unique_hlocal_ansistring sid_string;
    wil::unique_tokeninfo_ptr<TOKEN_USER> info;

    THROW_IF_FAILED(
        wil::get_token_information_nothrow(info, GetCurrentProcessToken()));
    THROW_IF_WIN32_BOOL_FALSE(
        ConvertSidToStringSidA(info->User.Sid, &sid_string));

    sec_desc = "D:P(A;;FA;;;SY)(A;;FA;;;";
    sec_desc += sid_string.get();
    sec_desc += ")";
    return sec_desc;
}

void hvtool::util::create_pipe(const std::string &pipe_name) {
    std::array<char, BUFF_SIZE> buf{};
    DWORD bytes_read = 0, bytes_written = 0, len = 0;
    HANDLE conout = GetStdHandle(STD_OUTPUT_HANDLE);

    wil::unique_hfile pipe(CreateNamedPipeA(
        pipe_name.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE, 1,
        BUFF_SIZE, BUFF_SIZE, NMPWAIT_USE_DEFAULT_WAIT, nullptr));

    THROW_LAST_ERROR_IF(
        ConnectNamedPipe(pipe.get(), nullptr) == FALSE
        && GetLastError() != ERROR_PIPE_CONNECTED);

    do {
        len = ReadFile(pipe.get(), buf.data(), (DWORD)buf.size(), &bytes_read, nullptr);
        WriteFile(conout, buf.data(), bytes_read, &bytes_written, nullptr);
    } while (len && bytes_read >= 0);
}
