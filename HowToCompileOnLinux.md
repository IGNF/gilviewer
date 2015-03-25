# Introduction #

This HowTo explains how to compile the GilViewer C++ library on Linux. Tests were made on Ubuntu Jaunty (32 and 64 bits).

# Install the dependencies #

Install the following tools or libraries:

  * Mercurial
  * CMake
  * Boost (>= 1.37)
  * wxWidgets (>= 2.8.8) and wxrc
  * Image libraries: tiff, jpeg and png

_**For Ubuntu Jaunty**_, just enter this command in a terminal:

```
sudo apt-get install build-essential mercurial cmake libboost1.37-dev libwxgtk2.8-dev wx-common libjpeg62-dev libtiff4-dev libpng12-dev
```

# Get the source #

Get the source with this command (it will create a directory "gilviewer" where you entered the command and download the source in it):

```
hg clone http://gilviewer.googlecode.com/hg/ gilviewer
```

You can also checkout the unstable version:

```
hg clone http://unstable.gilviewer.googlecode.com/hg/ gilviewer-unstable
```

# Compile and install the library #

## Run cmake ##

Go to the gilviewer directory. If the default configuration is enough, just enter this command:

```
cmake -DCMAKE_BUILD_TYPE:STRING=Release .
```

Or, if you want to tune some parameters:

```
ccmake .
--> follow the CMake instructions, in particular:
--> launch the configure step with 'c'
--> set CMAKE_BUILD_TYPE to Release (please respect the case) within CMake
--> tune the parameters you want
--> generate the configuration with 'g'
```

## Compile ##

Just run the following command (replace N by the number of cores or procesors you have):

```
make -jN
```

If the compilation process stops when generating resources.cpp, you can download the file [here](http://code.google.com/p/gilviewer/issues/detail?id=1). Just copy it in the _src/GilViewer/gui/resources_ folder.

## Install ##

**You need to install GilViewer library binary and headers only if you intend to use plugins or to use it as a developper (see [Developper Guide](http://code.google.com/p/gilviewer/wiki/GilViewerDevelopperGuide)).**

It is possible to install the GilViewer library binary and headers. **However, please note that some files in [Boost/GIL](http://opensource.adobe.com/wiki/display/gil/Generic+Image+Library) have been modified and will be replaced during installation.**
Just run the following command as _superuser_:

```
sudo make install
```

If you get errors, [contact us](mailto:gilviewer-dev@googlegroups.com) or [report a bug](http://code.google.com/p/gilviewer/issues/entry).


_Thanks to Adrien for providing a template for this HowTo ;-)_