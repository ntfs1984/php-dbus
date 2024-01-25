# php-dbus
PHP D-bus binding - to create Linux applications using Linux d-bus interface

This is PHP extension which allows to basic work with D-bus from PHP, without any external exec.

Installation:

## [PHP-CPP]

If you don't have this library - you should install it. This library allows to create PHP extensions using C++, without any headache with Zend.
Repository of it can be found here: https://github.com/CopernicaMarketingSoftware/PHP-CPP and official website is here: https://www.php-cpp.com/documentation/install

Step 1. **Clone php-cpp repo**
```bash
git clone https://github.com/CopernicaMarketingSoftware/PHP-CPP.git
```
Step 2. **Edit Makefile and replace correct path to php-config at PHP_CONFIG const if neccesary.**
```bash
cd PHP-CPP
nano Makefile
```
Step 3. **Make**
```bash
make
```

Step 4. **Install**
```bash
sudo make install
```

## [PHP-DBUS]

It's current library.

Step 1. **Clone php-dbus repo**
```bash
git clone https://github.com/ntfs1984/php-dbus
cd php-dbus
```

Step 2. **Edit Makefile and replace correct paths to INI_DIR (not neccesary if you will add extension you your php.ini manually), EXTENSION_DIR - where extension will be installed, and path to php-config**


```bash
nano Makefile
```

Step 3. **Make**
```bash
make
```

Step 4. **Install**

  a) You can use automated installation if you correctly set paths in Makefile
```bash
sudo make install
```

  b) Or you can install compiled extension php-dbus.so to proper extension directory
```bash
cp php-dbus.so /path/to/your/php-extensions-directory/
```
and add path to that extension to your using php.ini:
```bash
echo "extension = /path/to/your/php-extensions-directory/php-dbus.so" >> /your/php.ini
```

Step 4. **Check if your PHP loaded ths extension**
```bash
/path/to/your/php -i | grep dbus
```

## [Usage]

Features of this library will be improved time to time and I will create proper documentation, but for now:
```php
<?php

// Created Dbus class
$d = new Dbus; 

/* This method enable detail error explanation, sometimes with examples
For example with "false" error will looks as:
Error: unknown G_DBUS_TYPE.
With "true" the same error will looks as:
[PHP-DBUS error] Unknown G_DBUS_TYPE. Correct usage is: $Dbus->Open('G_BUS_TYPE_SESSION') or $Dbus->Open('G_BUS_TYPE_SYSTEM')
*/
$x = $d->Verbose(true); 

// Opening D-bus for work. Parameters are G_BUS_TYPE_SESSION or G_BUS_TYPE_SYSTEM
$d->Open("G_BUS_TYPE_SESSION"); 

// Getting all registered services to array
$x = $d->ListNames(); 

// Printing array
print_r($x); 

// Getting output of method GetAll from :1.646 (it's my Telegram path)
$x = $d->GetAll(":1.646","/StatusNotifierItem","org.freedesktop.DBus.Properties"); 

echo $x;

// Calling method Activate to :1.646 (still my Telegram)
$x = $d->CallMethod(":1.646","/StatusNotifierItem","org.kde.StatusNotifierItem","Activate","(ii)","0,0"); 

// Closing connection to free memory.
$d->Close(); 
?>
```
