#!/usr/bin/env python
import sys
import logging
import glob
import optparse
import xml.etree.ElementTree as et
import re
import random

LENGTH = 20
NUM = 1000

def _print_list(l):
   result = ""
   for cand in l:
      result += str(cand) + " "
   return result.strip()

def _main(args):
   for x in xrange(0, NUM):
      ballot = []
      check = [False] * LENGTH
      i = 0
      while i < LENGTH:
         rand = random.randint(1, LENGTH)
         if not check[rand-1]:
             check[rand-1] = True
             ballot.append(rand)
             i += 1
      sys.stdout.write(_print_list(ballot)+"\\n")

if __name__ == "__main__":
   retCode = _main(sys.argv[1:])
   sys.exit(retCode)
