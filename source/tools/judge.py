#!/usr/bin/env python

import sys
import logging
import glob
import optparse
import xml.etree.ElementTree as et
import re

_moduleLogger = logging.getLogger(__name__)

def _get_classes_header(cpp, header, writeFile):
   cppf = open(cpp, 'r')
   cppcontent = cppf.read()
   cppf.close()
   req = open(header, 'r')
   content = req.read()
   req.close()
   wfile = open(writeFile, 'w')

   m = re.findall(r'#include(?! ").*', cppcontent);
   for includes in m:
      wfile.write(includes+"\n");

   m = re.findall(r'(class.*?};)', content, flags=re.DOTALL)
   for cppClass in m:
      wfile.write(cppClass+"\n\n");

   m = re.findall(r'^(?!#include).+', cppcontent, flags=re.MULTILINE)
   for rest in m:
      wfile.write(rest+"\n");
   

def _parse_options(args):
   # Setup the required arguments for this script
   usage = r"""
usage: %prog [options]

Ex: 
> python %prog -s 
""".strip()
   parser = optparse.OptionParser(usage)
   parser.add_option(
     "-s", "--source",
     action="store", dest="source", metavar="SOURCE", default=[],
     help=""
   )
   parser.add_option(
     "--header",
     action="store", dest="header", metavar="HEADER", default=[],
     help=""
   )
   
   parser.add_option(
     "-o", "--out",
     action="store", dest="output", metavar="OUTPUT", default=[],
     help=""
   )
   debugGroup = optparse.OptionGroup(parser, "Debug")
   debugGroup.add_option(
      "--verbose",
      action="store", dest="verboseLevel", default='1',
      help="Verbose output level."
   )
   debugGroup.add_option(
      "--log-file", metavar="PATH",
      action="store", dest="logFile",
      help="Write the log to file (if specified)"
   )
   debugGroup.add_option(
      "--test",
      action="store_true", dest="test", default=False,
      help="Run doctests then quit."
   )
   parser.add_option_group(debugGroup)

   options, positional = parser.parse_args(args)
   if positional:
      parser.error("Unsupported positional arguments %r" % positional)

   loggingLevel = {
      0: logging.DEBUG,
      1: logging.INFO,
      2: logging.WARNING,
      3: logging.ERROR,
      4: logging.CRITICAL,
   }.get(int(options.verboseLevel), None)
   if loggingLevel is None:
      parser.error("Unsupported verbosity: %r" % options.verboseLevel)

   return options, loggingLevel


def _main(args):
   options, loggingLevel = _parse_options(args)

   logFormat = '(%(asctime)s) %(levelname)-5s %(name)s.%(funcName)s: %(message)s'
   logging.basicConfig(level=loggingLevel, format=logFormat)
   if options.logFile is not None:
      fileHandler = logging.FileHandler(options.logFile, "w")
      fileHandler.setFormatter(logging.Formatter(logFormat))
      root = logging.getLogger()
      root.addHandler(fileHandler)

   if options.test:
      import doctest
      print doctest.testmod()
      return 0

   _get_classes_header(options.source, options.header, options.output)

   return 0


if __name__ == "__main__":
   retCode = _main(sys.argv[1:])
   sys.exit(retCode)

