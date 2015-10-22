# DLLs #

In order to peek at what's been exported in a DLL, use the following commandline command:

```
dumpbin /exports <dll-file>
```

# Accessing DLLs from Python using ctypes #

This snippet shows how to load a DLL and call a function exported in it.

```
```

The functions should be exported using the following signature:
```
extern "C" __declspec(dllexport)
```

Classes are not supported at the moment.