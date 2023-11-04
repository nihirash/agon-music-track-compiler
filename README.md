# Agon Music Track Compiler

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/D1D6JVS74)

Important note!

It's still unstable! Use on your risk but feel free use any ideas from this code for building your own products!

There're a lot of checks are skipped. This compiler waits VERY(VERY!!!) exact file format!

## What is it?

Kind-of tracker but without user interface. 

It builts track file from plain text file with simple and human readable format.

For Agon-compatible computers there're simple music player that can be builtin into your assembly code and used as simple two calls(one starts playing music, another stops).

## Building compiler

I'm building it using GNU Makefile and clang compiler. But code so simple - so any modern C compiler should work with it.

If you're using macOS/Linux - just call `make` and it should build your compiler.

## Creating tracks

Currently tracks count is fixed and it's 4 tracks and written as plain text.

```
T10
;; Hello, world!
| C-1 28 | E-4 1F | G-4 1F | --- -- |
| E-1 -- | --- -9 | --- -9 | --- -- |
| G-1 -- | --- -4 | --- -4 | --- -- |
| B-1 -- | --- -0 | --- -0 | --- -- |
```

If line starts from letter `T` it's means that you'll specify some kind of tempo(how many frames per line should be used, music playback linked with VSync interrupt). `T10` means that every line will sounds 10 frames.

If line starts from symbol `;` - this line is just comment and there you can write almost everything. For example, what chord you'll arpegiate, number of bar or something like this. 

And line with scores devided by channel collumns. Each collumn contains:

```
C#3 <- note

1F <- Parameters where
1<--- Waveform number(see vdp documentation)
 F <- Volume(from 0 to F, hex number) 
```

As note can be used also `R--`(release - stop playing current note). And as placeholder for ANY paramenter(note with octave IS single parameter) can be used symbol `-` - it will say that nothing changed from past line. 

When you'll prepare your track - you can built it with compiler with command like this: 
```
./nmc example/blues player/track.bin
```

Track can be listened on Agon Light or in emulator(fab-agon-emulator plays them ok).

Using [ez80asm](http://github.com/envenomator/agon-ez80asm) you can built example player and if your compiled track named `track.bin` and placed in same directory as player sources - you'll got application that playing your music(until you'll reset your Agon).

## Contribution

Feel free contribute to the project in any way!

You like fix my english in readme? It's good.

You corrected code - that's perfect. 

You've written song and want share it as example - it's awesome! 

## License

Agon Music Track Compiler licensed with [Nihirash's Coffeware License](LICENSE).

It isn't hard to respect it.

Happy hacking!