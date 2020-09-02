#!/usr/bin/env python3

import math
import random
import shutil
import signal
import sys
import termios
import time

import serial

from drawille import Canvas
from serial.tools.list_ports import comports

from menu import Menu


OLD_TERM = termios.tcgetattr(sys.stdin.fileno())
BAUDRATES = [300, 2400, 9600, 19200, 38400, 57600, 115200, 230400]

def device_string(port, baud):
  dev = port.device
  desc = port.product
  return f'\x1b[38;5;4m{dev}\x1b[0m@\x1b[38;5;5m{baud}\x1b[0m ({desc})'

def warning(msg):
  goto(0, 0)
  print(
    f'\x1b[38;5;11mWARNING: {msg} [{time.perf_counter():.0f}]\x1b[0m',
    end='',
    flush=True
  )

def goto(x, y):
  print(f'\x1b[{y};{x}H', end='', flush=True)

def clear_screen():
  print('\x1b[2J', end='', flush=True)

def sigint_handler(*args):
  goto(0, 0)
  clear_screen()
  termios.tcsetattr(sys.stdin.fileno(), termios.TCSAFLUSH, OLD_TERM)
  sys.exit(0)

def main():
  signal.signal(signal.SIGINT, sigint_handler)

  cports = comports()
  ports = [p.device for p in cports]
  descs = [f'{p.product}' for p in cports]
  
  if not ports:
    print('No serial ports found.', file=sys.stderr)
    sys.exit(1)

  with Menu(ports, descriptions=descs, default_selection=0) as menu:
    port_i, port = menu.choose('Choose port:')

  with Menu(BAUDRATES, default_selection=BAUDRATES.index(115200)) as menu:
    _, baud = menu.choose('Choose baud rate:')

  w, h = shutil.get_terminal_size((80, 24))
  cport = cports[port_i]
  buf = ['0']
  cvs = Canvas()
  s = serial.Serial(port, baud, timeout=5)
  print('Opening', device_string(cport, baud))

  while True:
    line = s.readline().decode('ascii').strip()
    if line == '':
      warning('no data')
      continue
    else:
      buf.append(line)

    # Shift the data
    if len(buf) >= w*2:
      buf = buf[1:]
    
    # A list of all numbers from all plots in the buffer
    try:
      nums = sum([[float(x) for x in l.split(' ')] for l in buf], [])
    except ValueError:
      warning('NaN data')
      del buf[-1]
      time.sleep(0.5)
      continue

    # Plot the Braille lines
    goto(0, 0)
    clear_screen()
    cvs.clear()
    for i, l in enumerate(buf):
      l = [float(x) for x in l.split(' ')]
      for j, x in enumerate(l):
        # normalized value (adjusted to tty height)
        # h*4 because there are 4 Braille dots per row
        cvs.set(i, -x/max(nums) * (h*4-20))
    print(cvs.frame())

    # Attach some information
    print(f'{min(nums)}')
    print(device_string(cport, baud))
    print(f'{line}')
    print(f'\x1b[0;0H{max(nums)}', end='', flush=True)

if __name__ == '__main__':
  main()
