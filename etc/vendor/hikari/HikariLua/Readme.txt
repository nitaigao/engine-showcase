HikariLua library v0.1
Mark Manyen 

Users of Hikari can use this library to integrate the Lua scripting language into 
their projects and control Flash panels using Lua. 

The syntax of the Hikari library has been mostly preserved by using 
Lua's object oriented features. The exceptions are noted below.

	The bind function takes the name of the Lua function to bind (a string) 
	to the event instead of a FlashDelegate.

	There is no direct access to the HikariManager singleton (I may change this later), 
	so to create a FlashControl you instantiate it like any other Lua object 
	and then call the “init” member function. See example.

	The init function takes a string to designate the screen position of the 
	FlashControl instead of the enum. (Again this may change in the future).


Building:

You will need a recent version of Lua (http://www.lua.org) to build the library. 

I use environment variables to control the locations of include and library files. 
The following environment variables are required to build the library with my solution files.

LUA_HOME (ex. C:\code\Open Source\Lua\source\lua-5.1.2)
HIKARI_HOME (ex. C:\code\Open Source\Hikari\Hikari)
OGRE_HOME (set to the Ogre SDK directory, if you build Ogre from source you can figure it out)


Extras:

I have included release and debug Lua libraries and DLLs and the Boot.lua file 
I use for the demo project in the “extras” folder.

HikariLuaDemo:

I have recreated the original HikariDemo to show off the library. 
The HikariLuaDemo project requires that the “Boot.lua” file from the extras folder be 
placed in the Hikari “bin” folder and the an appropriate Lua DLL is available. 
It should be the same as the Hikari demo, except the FPS isn't updated. I'll leave that 
as an exercise for the user (Hint: Timers!).

License:

While the LGPL licence is very good, I don't think this code needs to be in a separate DLL, 
so I choose to use the less restrictive MIT license, pasted below.

Copyright (c) 2008 Mark Manyen and The Johnson Simulation Center

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

