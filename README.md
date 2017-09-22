# Pacific Kernel

A minimalist C kernel. 

At present the kernel does the following:
* Boots up in an emulator
* Sets up a GDT table
* Jumps to long mode
* Prints to screen some basic messages
* Handles interrupts and exceptions
* Counts time upwards
* Handles keyboard input

The code for this kernel is far from polished. At the moment there is lack of structure, few comments, and multiple warnings I've ignored. Do not use this as a reference for what good code looks like. At this point in time the purpose of this kernel is to serve as a scratchpad for myself to learn how kernels really work in a hands-on manner.

To get to this point I used the following resources:

https://os.phil-opp.com/multiboot-kernel/ (Excellent tutorial for learning the basics, but I diverged when the author switched to Rust)
http://wiki.osdev.org/Expanded_Main_Page (Lots of great info here even if much of the wiki spends time explaining how scary OS development is)
http://www.osdever.net/bkerndev/Docs/intro.htm (A little outdated, but a great step by step overview of the different parts of a basic kernel)