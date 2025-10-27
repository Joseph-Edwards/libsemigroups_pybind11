#!/bin/bash
set -e

echo "Constructing requiremens list and writing to requirements.txt ..."

# The following requires pip-compile, which is made available through the package pip-tools
CUSTOM_COMPILE_COMMAND="./etc/$(basename $0)" pip-compile --upgrade --all-build-deps --all-extras --strip-extras pyproject.toml
echo "Done"
echo

echo "Copying requirements into requirements-archive/ ..."

VERSION=$(python -c "import sys; print(sys.version_info[1])")
cp requirements.txt requirements-archive/py$VERSION-requirements.txt 
echo "Done"
