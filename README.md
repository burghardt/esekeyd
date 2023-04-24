esekeyd
=======

ESE Key Daemon - multimedia keyboard driver for Linux

## Getting started

These instructions will get you a copy of the project up and running on your
local machine.

### Building from source

#### Prerequisites

Compile kernel with event interface input support. Generic kernels should have
this enabled by default.

If `evdev` was compiled as a module (probably the best choice) load the module
with

    sudo modprobe evdev

You may want to keep the default permissions for keyboard event devices.
If you need to change anything, remember to set access correctly to prevent
users from installing key loggers.

#### Downloading

Clone repository

    git clone https://github.com/burghardt/esekeyd.git


#### Building

Compile program using the following commands

    sh bootstrap
    ./configure
    make

#### Installing

Optionally install program

    sudo make install

### Usage

There are three commands available: `keytest`, `learnkeys` and `esekeyd`.

Use `keytest` to test if additional "multimedia" keys of your keyboard are
recognized. If so, create a skeleton configuration file

    learnkeys ~/.config_file_name.conf

Edit this file to assign a command to each key that will be executed when
the key is pressed.

Finally, run the key daemon and check that the commands for each key are
executed.

    esekeyd ~/.config_file_name.conf

You don't have to run it as root if you set the permissions correctly,
if you need to do root things (like `init 5`) use `sudo`.

My config file is included in examples directory for reference.

## Acknowledgments

This program was inspired by [discussions about Funkey and 2.6 Linux
kernels](http://lkml.iu.edu/hypermail/linux/kernel/0309.3/1463.html).
I just read what Vojtech Pavlik thought about the Funkey patch
and implemented what he has suggested.
