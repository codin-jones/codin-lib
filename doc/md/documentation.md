# Documentation

I use [doxygen](http://www.stack.nl/~dimitri/doxygen/) to generate the html and 
man pages for codin-lib.


### Doxygen generated [HTML](https://b897d9d10db91855b152fd79f76e83824f34aa29.googledrive.com/host/0B1ztEpdpMB0JYjhmb1JHbEx0emc/index.html) API

As I stated in the [Background](../../README.md) I like to jump right into the 
header files to learn the interfaces, however I realize normal people like to 
read some documentation in their browsers.

### Generated man pages

The generated man pages are in section 3 of the manual. The only weird part, 
in my opinion, is that you need to replace any scoping ```::``` with ```_``` 
to get the proper man page. So to get the man page for ```cdn::thread::DataGuard``` 
you need to perform the command ```man cdn_thread_DataGuard```

### Sample usage

I developed a unit test for each class, refer to them for example usage.
Alternately you can refer to the [Examples](https://b897d9d10db91855b152fd79f76e83824f34aa29.googledrive.com/host/0B1ztEpdpMB0JYjhmb1JHbEx0emc/examples.html) 
under the doxygen documentation for some simple examples.
