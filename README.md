# Cerverus
Cerverus is a webserver built in C. It uses minimal libraries and can run multi-platform.

It is still a work in progress with an youtube series in where it is built. (Still figuring out how to start it and stuff...)

NOTE: not all features have been added, as you can see there is no current release.

## Architecture
### Receiving a Request
An socket connection is requested, it is accepted if it isn't the subject of rate limiting (maybe feature?). The connection is accepted by the server object, it then parses the request into an request object. The request object is then handed on to the router which finds the correct handler, and then handles it (maybe implement a type of command buffer so that networking stays in the server layer?).

## Credits
The code was inspired by, copied from, references, and/or is connected to:
 - [Kohi Game Engine, by Travis Vroman](https://github.com/travisvroman/kohi)
 - [Making Minimalist Web Server in C on Linux, by Nir Lichtman](https://youtu.be/2HrYIl6GpYg)