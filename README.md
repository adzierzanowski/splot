# splot

This utility is a text mode implementation of what can be done in Arduino IDE
when choosing `Serial Plotter` option, i.e. it plots the numbers received on
the serial port in real time.

## Requirements

This program assumes that your terminal is POSIX-compliant, supports colors
and your font of choice has Braille characters. Module requirements are
specified in `requirements.txt`.

## Disclaimer: Work in Progress

This is a dirty draft of what I want it to be. I'll try to optimize it in my
free time and add some colors to differentiate the plots. As for now, it is
what it is.

The initial menu is made in TUI style but i plan to add an option to get rid
of it if you want to use plain old switches instead.

**Oh, and the menu has vi-style navigation. `J` is down, `K` is up!!!**

This will also be handled later.

## Screenshot

![](https://i.imgur.com/37MfwQr.png)
