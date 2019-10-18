#!/usr/bin/env bash

ls -la /usr/local/opt
ls -la /usr/local/Cellar

brew update
brew install python3
sudo python3 -m pip install nose2
sudo python3 -m pip install twine

brew install ninja
brew install pyenv
pyenv install 3.8.0
pyenv install 3.7.4
pyenv install 3.6.8
