# codin-lib

# Skip all the background and jump to the [Documentation](/doc/md/documentation.md)

### Background

I developed codin-lib because even though C++11/14 provides a much richer set of
libraries than C++98/03 did there are still some utilities that I found I needed.
Some of these classes have similiar libraries in the [boost](http://www.boost.org) 
library, which I would recommend everyone who writes C++ to become familiar with.

In case you are wondering why I call it codin-lib, it is because that is how I describe
what I am doing.<br/>
I'm codin.

I developed my own library instead of contributing to boost for a few reasons.

### I am not targeting Windows

I am not going to put in time to ensure that my library runs under Windows, since
my needs are for Linux and Mac OS X. If someone wants to ensure it works
under Windows then I would be happy to accept any modifications that are necessary.
That being said I strive to make sure that I write standard C++ that any compliant
compiler should be able to compile. I am going to the codin library compliant to the
last released standard, since not everyone can be at the latest version. In other
words right now C++14 is out so I will only use features from C++11. When C++17 
comes out then I will move to using features from C++14 and so on.

### The code I write at my day job isn't allowed to be publicly available.

Therefore the code I write here is something I could actually show other people and
discuss in an open forum.

### Make the classes simple to  understand a the interface well documented right in the header file

For every project I have ever used, I jump right in the headers for the library and
start wandering around to understand the interface. Yes, I also use the examples
to help me get started, but I quickly want to read the associated headers (and 
source if it's available) to gain a deep understanding of how a class works. 

My theory at work, and with codin-lib, is that if a client has to open up the source
file to understand the public interface of the class then that class is not 
documented well enough. I feel it is perfectly reasonable for a maintainer to need 
to use the source file to gain the depth of understanding required to add or update
functionality.