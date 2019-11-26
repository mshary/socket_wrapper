# socket_wrapper
A library that overrides the socket options set by any pre-existing application

Linux shared libraries (with extension .so) are load and unload dynamically in system memory as needed.
This project demonstrates how any method contained in a shared library can be override at run-time to 
execute arbitrary code before that shared library is loaded into memory.

This library specifically modify socket options for any new TCP connection that is about to be created
by any preexisting program. The program is not aware of those options nor able to react to them unless
it is modified and recompiled.

## Use Cases:

1. To set specific ToS and QoS values for critical network communication programs such as SSH, WebRTC
and DB cluster services etc.

2. To enable TCP extensions such as Multi-Path TCP (MPTCP) for pre-existing programs, provided that
the newer socket library (i.e. with MPTCP support) is backward compatible. This means e.g. an SSH client
application linked against older GNU C shared library (without MPTCP support) can utilize MPTCP capabilities
of newer GNU C shared library without having to be modified and recompiled.

3. It can be extended to analyze, hack or reverse engineer any existing program.

## Build Steps

### Dependencies
* C compiler (C99 compliant)
* Linux kernel MPTCP user API headers (optional)

### Compilation
Simply run `make`. It will generate `libsocket_wrapper.so` file with MPTCP support.
To compile without MPTCP support run `make without_mptcp`.

### Usage
Set the environment variable `LD_PRELOAD` with full path to `libsocket_wrapper.so` before running the program.

e.g.

`LD_PRELOAD=/home/shahzad/shari/socket_wrapper/libsocket_wrapper.so ssh localhost`

This will print logs like below in stderr, indicating the socket methods that have been successfully applied on
the program.

```
Loading __close => 0x7f83be2308c0
Loading __read => 0x7f83be230070
Loading __socket => 0x7f83be242ec0
Loading __connect => 0x7f83be2428a0
Loading __setsockopt => 0x7f83be242e60
Loading __write => 0x7f83be230140

```


