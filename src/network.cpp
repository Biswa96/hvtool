// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#include <winsock2.h>
#include <hvsocket.h>
#include <iostream>
#include <string>

#include "wil/resource.h"

#include "network.h"
#include "util.h"

void hvtool::network::run_server(const std::string &vm_id) {
    WSADATA wsa_data{};
    SOCKADDR_HV addr{};
    ULONG optval = true;
    std::string buf;

    THROW_LAST_ERROR_IF(WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0);

    wil::unique_socket sock(WSASocketW(
        AF_HYPERV, SOCK_STREAM, HV_PROTOCOL_RAW, nullptr, 0, 0));

    THROW_LAST_ERROR_IF(setsockopt(
        sock.get(), HV_PROTOCOL_RAW, HVSOCKET_CONNECTED_SUSPEND,
        reinterpret_cast<const char *>(&optval), sizeof(ULONG)) != 0);

    addr.Family = AF_HYPERV;
    memcpy(&addr.ServiceId, &HV_GUID_VSOCK_TEMPLATE, sizeof(GUID));
    addr.ServiceId.Data1 = hvtool::DEFAULT_PORT;

    THROW_LAST_ERROR_IF(UuidFromStringA(
        (RPC_CSTR)vm_id.c_str(), &addr.VmId) != RPC_S_OK);

    THROW_LAST_ERROR_IF(bind(sock.get(),
        reinterpret_cast<const sockaddr *>(&addr), sizeof(SOCKADDR_HV)) != 0);

    THROW_LAST_ERROR_IF(listen(sock.get(), SOMAXCONN) != 0);

    wil::unique_socket client(accept(sock.get(), nullptr, nullptr));

    while (true) {
        std::cout << "Enter message (type 'exit' to quit): ";
        std::getline(std::cin, buf);
        if (buf == "exit") {
            break;
        }
        if (send(client.get(), buf.c_str(), (int)buf.size(), 0) == SOCKET_ERROR) {
            LOG_LAST_ERROR();
            break;
        }
    }
}
