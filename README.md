setproctitle-php
================

A PHP extension to provide `setproctitle()` for CLI apps.
With all the black magic it takes to do it on Linux.

Compilation
-----------

    $ phpize
    $ ./configure --enable-setproctitle
    $ make
    # make install

And have your php.ini file load the extension:

    extension=setproctitle.so

Usage
-----

    setproctitle('w00t');

