import sys
import os


# Add the source directory so that packages can be imported in tests
source = os.path.abspath(os.path.relpath('../source'))
sys.path.insert(0, source)