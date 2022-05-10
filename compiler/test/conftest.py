import sys
import os


# Add the sourcedirectory so that packages can be imported in tests
source_path = os.path.abspath(os.path.relpath('../source'))
sys.path.insert(0, source_path)
