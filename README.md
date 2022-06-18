# Hack VM
Stack-based virtual machine translator for translating HACK VM code ➡️ [HACK assembly](https://en.wikipedia.org/wiki/Hack_computer#Instruction_set_architecture_(ISA)_and_machine_language). This idea came from the proposals in Project 7-8 of the amazing [Nand2Tetris](https://www.nand2tetris.org/) project.

I've chipped away at building it every so often, and it was one of the most fun pet projects I've taken on. It also resulted in some of the worst code I've written (I hadn't written a line of C++ when I first started) but I came away with very practical insight into how high-level programs are laid out in assembly...Which has been invaluable knowledge.

I highly encourage anyone looking into the Nand2Tetris project to jump in! You won't regret it.

## Build
Compile the translator:
```
~/workspaces/repo
$ cmake --build build
$ make -C build # build
```

## Test
Run translator tests (CPU emulator tools provided by Nand2Tetris):
```
$ make -C build test # run tests
```
## Run
Translate `*.vm` file(s) to HACK assembly:
```
$ ./build/vm_translator Sys.vm sys.asm # Translate Sys.vm -> sys.asm
$ ./build/vm_translator ./some/directory/ sys.asm # Translate *.vm files in directory -> sys.asm
```
