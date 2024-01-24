# pycsdr-eti

This project contains python bindings for the
[csdr-eti](https://github.com/jketterl/csdr-eti) library.

# Installation

The OpenWebRX project is hosting pycsdr-eti packages in their repositories. Please click the respective link for [Debian](https://www.openwebrx.de/download/debian.php) or [Ubuntu](https://www.openwebrx.de/download/ubuntu.php). Due to naming conventions, the repository package is called `python3-csdr-eti`.

# Compiling from source

Please install `libcsdr-eti` and its dependencies before compiling this project.

Please also install the python development files (`libpython3-dev` on most Debian-based distributions).

```
sudo ./setup.py install install_headers
```
