# Documentation

I use [doxygen](http://www.stack.nl/~dimitri/doxygen/) to generate the html and 
man pages for codin-lib.


### Doxygen generated [HTML](https://345debf3e473676794b2679187fb17c6e1a63d38.googledrive.com/host/0B1ztEpdpMB0JR2g0ek1SRzNrZnM/annotated.html) API

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
Alternately you can refer to the [Related Pages](https://345debf3e473676794b2679187fb17c6e1a63d38.googledrive.com/host/0B1ztEpdpMB0JR2g0ek1SRzNrZnM/pages.html) 
under the doxygen documentation for some simple examples.
