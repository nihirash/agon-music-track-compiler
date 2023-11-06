# Background music player

It's proof of concept moslet that working in background and playing music that built with 
Agon Music Track Compiler.

## Usage

Copy `bgplay` to `/mos/` directory of Agon's SD card and call it `bgplay <trackname>` for starting playing music.

If you'll call routine placed by `0x90004` address - it will restore interrupt and stop playing music.

## Development

Use [ez80asm](http://github.com/envenomator/agon-ez80asm) and built `resident-part.asm` first and after it - `bgplay.asm`. 

