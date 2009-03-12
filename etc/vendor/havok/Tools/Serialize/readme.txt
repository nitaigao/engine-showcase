

What are these files?
=====================

This directory contains code to parse havok header files and generate
reflection data from them.

You will need python2.3 or higher to run these scripts. Get it from www.python.org.
See also http://www.python.org/topics/learn/ for an overview.

See the userguide for more details.


Scripts Overview
===============


regenerateXml.py
    Recursively search a directory for class metadata to update.
    Use this script if you have changed a class definition.


bindingtoXml.py
    This file converts a python xml data binding back into xml.
    See hkcToDom for an example data binding.

domToClass.py
    This script is used to generate an c++ hkClass instance from an xml data binding.
    All the *Class.cpp files in the havok tree are generated from this script.

domToHeader.py
    This script is used to generate a c++ header file from an hkClass xml dom.
    Now that we can extract xml directly from the c++ header, this file is
    of diminishing utility.

genRegister.py
    Generates a c++ array of hkClass pointers which can be used to populate an
    hkClassRegistry.

headerToDom.py
    Script is used to extract an hkClass xml definition from a c++ header file.

hkcToDom.py
    Data binding interface. Converts xml into python objects for easier manipulation.

mergeBind.py
    Script to merge two data bindings. This script ensures that changes to the
    hkclass files are propagated each time the header is scanned with headerToDom.py

typeMapping.txt
    Type aliases used in domToClass
