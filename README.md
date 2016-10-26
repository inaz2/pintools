# pintools

Works with [Intel Pin](https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool) Rev 76991 (Mar 24, 2016).

## Example

```
$ ls ../
doc/  extlicense/  extras/  ia32/  intel64/  LICENSE  pin*  pintools/  README  redist.txt  source/

$ ../pin -t obj-intel64/inscount.so -- /bin/ls
cmptrace.cpp  inscount.cpp  inscount.out  livestrings.cpp  obj-intel64  README.md

$ cat inscount.out
379607
```
