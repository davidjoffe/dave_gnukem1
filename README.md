# Dave Gnukem
Dave Gnukem is a 2D scrolling platform shooter similar to, and inspired by, Duke Nukem 1 (~1991). The source code is cross-platform and open source. It runs on Windows, Linux, Mac OS X and more. (The game is intentionally somewhat 'retro'; the original Duke Nukem 1 had 16-color EGA 320x200 graphics.)

Source code repository: https://github.com/davidjoffe/dave_gnukem

**Release Downloads:**: https://sourceforge.net/projects/gnukem/

Old project page: http://gnukem.sourceforge.net/

Created by: David Joffe http://djoffe.com/ (https://www.livecoding.tv/david_joffe/)

The project was maintained by EMH (Evil Mr Henry) for a number of years until Oct 2016

You can watch recent development on this project being live-streamed at:
https://www.livecoding.tv/david_joffe/
e.g.
https://www.livecoding.tv/david_joffe/videos/wae18-dave-gnukem-c-game-side-scrolling-shooter and 
https://www.livecoding.tv/david_joffe/videos/K9XBz-dave-gnukem-c-game-side-scrolling-shooter-3 etc.

![Dave Gnukem Screenshot](http://i.imgur.com/mN12Vtk.png "Dave Gnukem Screenshot")

## About Duke Nukem 1
Duke Nukem 1 was a famous original 16-color 320x200 'classic' game released by Apogee Software in 1991 that launched the Duke Nukem series: https://goo.gl/yP4PbS  The original Duke Nukem 1 was created by Todd Replogle (co-creator of the Duke Nukem series), John Carmack (of id Software), Scott Miller (founder of 3D Realms), Allen H. Blum III, George Broussard, and Jim Norwood.

## Gameplay Instructions

Try find the exit, while dodging or shooting monsters.

### Default Keys:

* Left/Right: Move left/right
* Ctrl: Jump
* Alt: Shoot
* Up Arrow: Action key (for e.g. opening doors, using teleporters or lifts, activating the exit, etc.)
* Escape: In-game menu

* PgUp/PgDn: Increase/decrease volume

To open doors, find the correct color key, and press the action key on the 'lock' nearby the door(s).

Power boots allow you to jump higher.

# History / Changes

2016/10/22 - Version 0.63 / 0.64 / 0.65

* Add background music. So far, have only added music by Eric Matyas http://soundimage.org/
* Add game volume controls (use PgUp and PgDn keys to increase/decrease volume from menus or in-game)
* Level Editor: New feature: Hold in Ctrl+Alt and click with the mouse to automatically start level with hero 'dropped in' to the clicked position as starting position (to help with level editing / testing)

2016/10/16 - Version 0.62

* Get Dave Gnukem compiling and running on Mac OS X: https://www.livecoding.tv/david_joffe/videos/jbBvb-dave-gnukem-c-game-side-scrolling-shooter-9
* Miscellaneous minor aesthetic improvements

2016/10/10 - Version 0.61

* Fix: "Dying in the pungee sticks will often cause a crash" (thank you to porter who pointed that out)
* Fix a segfault in Linux (possibly VNC only?) when navigating in-game menus and other sub-menus
* Add semi-experimental 'big viewport mode' (Backspace + B in game to toggle) to make level creation/testing easier for level editors

2016/10/09 - Version 0.6 (0.60)

* Take the 2015-08-01 v0.56a EMH release from SourceForge, create github project https://github.com/davidjoffe/dave_gnukem
* Fixed 'tiny game window'
* Fix some Linux compile issues, make a few other fixes/improvements (streamed on LiveCoding), call it v0.6 ('un-abandoning' project - DJ)


# Developer Info / Build Info

Dependencies: LibSDL1.2, LibSDL-Mixer 1.2

## Windows Build Instructions

* Visual Studio 2010 project is included.
* **NB** You must set your Visual Studio 'Working Directory' to ../../ in order to run this (under Project / Properties / Configure Properties / Debugging) or it won't find the data files and just immediately exit on run
* If you get an error about not finding SDL DLL(s) when running, set 'Project / Properties / Configure Properties / Environment' to: PATH=c:\your\path\to\DLLs;%PATH%

## Linux Build Instructions

Type 'make'

Run with ./davegnukem if it built correctly

Installing dependencies on Debian etc.:

apt-get install libsdl1.2-dev

apt-get install libsdl-mixer1.2-dev


## Mac OS X Build Instructions

Same as Linux - type 'make'. Run with ./davegnukem if it built correctly.

Dependencies: You may have to first install LibSDL1.2 and LibSDL-Mixer1.2 (these can be installed by downloading the source code and doing 'make' and 'make install' (as root) for each of these first).

# TODOs (To Sort):

* Create 'Roadmap' to "version 1"? E.g. starter list below:
* Videos?
* Check through code for possible buffer over-runs and 
* v0.7 [stil issue with ctrl jump keypress story]
* v0.7 linux segfault
* 'Texture Manager':
	* Clean up of 'textures'
	* Actual hardware accel if available [todo: proper blending]
	* Restoring hardware surfaces on toggle fullscreen with DirectX
* Explosion sound doesn't always play (fixed 2016-10-22 but fix could use more testing, cf. channel -1 story)
* DEV
	* Update TargetName to DaveGnukem in VS2010
	* Set up working folders to work 'out the box', so to speak
	* Add a dev readme.txt to help devs get set up

* [med] Level Editor is hogging CPU [check sprite editor, does it also?]
* Finalize sprites and other graphics to at least reasonable ready state
* Finalize a basic playable set of levels
* To make it easier to edit levels, need
	some way to easily 'place' hero
	at the map area you're editing
* Check for all possible hardcoded keys that may be interfering with redefined keys (e.g. Ctrl, H, backspace+G etc.)
* [After redefine keys] Ctrl *both* shoot and jump
* H to damage health should not be on by default [likewise other things]
* Better colors for menu
* Some more / better sounds
* CREDITS: Add Vytautas Shaltenis, a.k.a. rtfb, EMH, etc. [who else?]
* Some better graphics
* Fix up and update credits etc.
* Signed executables in binary release?
* [check] Dying seems to reset score to 0 - is that right? Cf. DN1 behavior
* Is the -640 flag now basically obsolete? Remove? Or change to do something else useful? E.g. to make window maximum size e.g. same as monitor could be useful for level editing.

## LEVEL/SPRITE EDITOR TODOS:

* Nice to have: If have big screen like 1920x1080, should be able to 'take advantage' to make it nicer to edit levels [partly done]
* If unsaved changes and accidentally press escape, your changes are lost .. should track 'dirty' and ask are you sure, or something along those lines.
* [nice to have?] Undo last action?
* It seems very easy to click sprite in wrong location
* [nice to have?] Sort of a 'sprite eye-dropper' type tool: Point mouse and press something (e.g. Alt?) to 'pick up' what the mouse is floating over, and set that into the 'sprite palette'

## Maybe make videos (e.g. for youtube etc.?) on:
	
* Overview of game
* Overview level editor
* Level editor howto

## Sprite TODOs:

* Some of the background brick colors should be a bit darker
* Some smoothing on some of hard pixely 'lines' (e.g. on shadow edges)
* Finish conveyor belt
* Un-EGA-ish sprites that still look a bit too EGA-ish (some of the especially ones look a bit too 16-color-ish perhaps, though the original DN1 was 16 colors)
* Red background pipes look a bit .. I dunno what .. chromatic? Maybe desaturate slightly .. or something

## Level TODOs:

* ? Make a list of 'level TODOs' that are must-have for a 'version 1'?


