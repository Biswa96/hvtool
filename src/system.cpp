// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#include <windows.h>
#include <computecore.h>
#include <fstream>
#include <iostream>
#include <thread>

#include "wil/resource.h"
#include "nlohmann/json.hpp"

#include "network.h"
#include "system.h"
#include "util.h"

using json = nlohmann::json;

using unique_hcs_operation = wil::unique_any<
    HCS_OPERATION, decltype(&HcsCloseOperation), HcsCloseOperation>;

using unique_hcs_system = wil::unique_any<
    HCS_SYSTEM, decltype(&HcsCloseComputeSystem), HcsCloseComputeSystem>;

std::string hvtool::system::get_vm_id(const std::string &vm_name) {
    json jsdata;
    std::string vm_id;
    wil::unique_hlocal_string result;

    unique_hcs_operation operation(HcsCreateOperation(nullptr, nullptr));

    THROW_IF_FAILED(HcsEnumerateComputeSystems(nullptr, operation.get()));

    THROW_IF_FAILED_MSG(
        HcsWaitForOperationResult(operation.get(), INFINITE, &result),
        "get_vm_id result: %ws", result.get());

    jsdata = json::parse(hvtool::util::to_bytes(result.get()));

    for (const auto &item : jsdata) {
        if (item.contains("Owner") && item["Owner"] == vm_name) {
            std::cout << item.dump(2) << "\n";
            vm_id = item["RuntimeId"];
        }
    }

    return vm_id;
}

void hvtool::system::create_vm(const std::string &json_file) {
    unique_hcs_system system;
    wil::unique_hlocal_string result;

    THROW_LAST_ERROR_IF(
        GetFileAttributesA(json_file.c_str()) == INVALID_FILE_ATTRIBUTES);

    std::ifstream ifs(json_file);
    json jsdata = json::parse(ifs, nullptr, true, true);
    const std::string vm_name = jsdata["Owner"];

    const std::string sec_desc = hvtool::util::sid_string();
    jsdata["VirtualMachine"]["Devices"]["HvSocket"]["HvSocketConfig"]
          ["DefaultBindSecurityDescriptor"] = sec_desc;
    jsdata["VirtualMachine"]["Devices"]["HvSocket"]["HvSocketConfig"]
          ["DefaultConnectSecurityDescriptor"] = sec_desc;

    const std::string com_pipe_name = jsdata["VirtualMachine"]
        ["Devices"]["ComPorts"]["0"]["NamedPipe"];
    const std::string serial_pipe_name = jsdata["VirtualMachine"]
        ["Devices"]["VirtioSerial"]["Ports"]["0"]["NamedPipe"];

    auto com_pipe_thread = std::thread([&]() {
        hvtool::util::create_pipe(com_pipe_name);
    });

    auto serial_pipe_thread = std::thread([&]() {
        hvtool::util::create_pipe(serial_pipe_name);
    });

    unique_hcs_operation operation(HcsCreateOperation(nullptr, nullptr));

    THROW_IF_FAILED(HcsCreateComputeSystem(
        hvtool::util::from_bytes(vm_name).c_str(),
        hvtool::util::from_bytes(jsdata.dump()).c_str(),
        operation.get(), nullptr, &system));
    THROW_IF_FAILED_MSG(
        HcsWaitForOperationResult(operation.get(), INFINITE, &result),
        "create system result: %ws", result.get());

    THROW_IF_FAILED(
        HcsStartComputeSystem(system.get(), operation.get(), nullptr));
    THROW_IF_FAILED_MSG(
        HcsWaitForOperationResult(operation.get(), INFINITE, &result),
        "start system result: %ws", result.get());

    hvtool::network::run_server(get_vm_id(vm_name));
    com_pipe_thread.join();
    serial_pipe_thread.join();
}
