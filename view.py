#!/usr/bin/python3

import sys
from sys import *
import os
from subprocess import call


if __name__ == "__main__":

    username = argv[1]
    stream = argv[2]
    index = argv[3]
    order = argv[4]
    command = argv[5]
    
    call(["./db", command, username, stream, index, order])    
