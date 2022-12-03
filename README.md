# mouse-emulator
Listens to keyboard input events and injects input events into a mouse device on linux

A physical mouse needs to be connected

Keycodes defined in [linux/input-event-codes.h](https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h)

```
Usage: mouse-emulator [OPTION...] --input [FILE] --output [FILE]
Emulates mouse actions with keyboard keys
 Keycodes defined in linux/input-event-codes.h

  -i, --input=Input          Input device (keyboard) path
  -l, --mouseleft[=MouseLeft]   Keyboard key code for LMB
  -L, --mouselefthold[=MouseLeftHold]
                             Keyboard key code for holding LMB
  -m, --mousemiddle[=MouseMiddle]
                             Keyboard key code for MMB
  -M, --mod[=ModKey]         Keyboard key code for modifier key
  -o, --output=Output        Output device (mouse) path
  -r, --mouseright[=MouseRight]   Keyboard key code for RMB
```
