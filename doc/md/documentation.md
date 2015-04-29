# Documentation

I use [doxygen](http://www.stack.nl/~dimitri/doxygen/) to generate the html and man pages from codin-lib.


### Doxygen generated [HTML](doc/output/html/index.html) API

As I stated in the [Background](../../README.md) I like to jump right into the header files to learn the interfaces, however I realize normal people like to read some documentation in their browsers.

### Generated man pages

The generated man pages are in section 3 of the manual. The only weird part, in my opinion, is that you 
need to replace any scoping ```::``` with ```_``` to get the proper man page. So to get the man page for ```cdn::thread::DataGuard``` you need to perform the command ```man cdn_thread_DataGuard```

### Sample usage

I develop a unit test for each class, so those are what you can use for example usage.