# hvtool

Tool to play with Linux virtual machine using Hyper-V Compute System APIs.

## Prerequisites

* Latest Microsoft Windows 10 or above.
* A Windows user in Hyper-V Administrators group.
* Latest Visual Studio with cmake.
* Latest stable Alpine Linux in WSL2.
* cpio, make, wget and gcc/clang in Alpine Linux WSL2.

## How-to build

* Clone this repository with git submodules.
* Open Developer Command Prompt from Start menu.
* Build hvtool.exe: `cmake -S . -B bin && cmake --build bin`
* Build initrd.img in WSL2: `make -C init`

## How-to use

* Add full path of Linux kernel and initrd.img in KernelFilePath and InitRdPath
  keys in [hvtool.json](./hvtool.json) respectively. e.g. `C:\\dir\\kernel`
* Place `hvtool.json` in current working directory.
* Run `hvtool.exe` to start the VM.
* Use putty to open console in `\\.\pipe\hvtool_serial_port1` serial line.
* Type "exit" in the running hvtool.exe console to shutdown the VM.

## Acknowledgments

* [Hyper-V Compute System API Example](https://learn.microsoft.com/en-us/virtualization/api/hcs/reference/tutorial)

## Contribution

Any ideas to improve this project is always welcomed.

## License

This project is licensed under GPL-3.0 or later. See [LICENSE](./LICENSE.txt).
