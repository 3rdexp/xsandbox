# DirectUI #
added Bjarke Viksoe`s DirectX 3D animation to duilib

# Axel-WIN32 #
porting to NATIVE WIN32 without Cygwin, what I did in the #if WIN32 ... #endif, for example:
  * Changed Linux file descriptor for the new socket to WIN32 SOCKET descriptor referencing the new socket;
  * Changed close, write, read to closesocket, send, recv;
  * Changed pthread\_t for the thread to HANDLE referenced the thread;
  * Changed pthread\_create, pthread\_join, pthread\_cancel to CreateThread, WaitForSingleObject, TerminateThread;