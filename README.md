esekeyd
=======

ESE Key Daemon is a multimedia keyboard driver for Linux.

[![Travis-CI](https://travis-ci.org/burghardt/esekeyd.svg?branch=master)](https://travis-ci.org/burghardt/esekeyd/)
[![CircleCI](https://circleci.com/gh/burghardt/esekeyd/tree/master.svg?style=svg)](https://circleci.com/gh/burghardt/esekeyd)
[![Coverity](https://scan.coverity.com/projects/6006/badge.svg)](https://scan.coverity.com/projects/6006)

How to use this program?
------------------------

1. Compile kernel with event interface input support.
(generic kernels should have it)

2. If compiled as a module (best choice) load module.
(type "modprobe evdev" as root)

3. If you know what permissions are set it correctly ;-P
(e.g. prevent users from installing key loggers)

4. Compile program.
(type "./bootstrap ; ./configure ; make")

5. Install program.
(type "make install" as root)

6. Run keytest and test if your keys works.
(type "keytest")

7. Run learnkeys and make config file.
(type "learnkeys config_file_name")

8. Edit config file generated by learnkeys.
(use your favorite editor)

9. Run it with the config file name.
(type "esekeyd config_file_name",
you don`t have to run it as root if you set permissions correctly,
if you want to do root need things (like init 5) use sudo,
my config file is included in examples directory)

10. Have fun!

This program was inspired by [discussion about Funkey and 2.6 Linux kernels](http://lkml.iu.edu/hypermail/linux/kernel/0309.3/1463.html). I just read what Vojtech Pavlik think about Funkey patch and implemented what he has suggested.
