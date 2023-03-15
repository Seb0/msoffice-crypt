# A tool/lib to encrypt/decrypt Microsoft Office Document

# Environment

* 64-bit Windows Visual Studio 2012 or later
* gcc 4.6, clang 3.0 or later
* libssl
  - `sudo apt install libssl-dev`

# How to make `bin/msoffice-crypt.exe`

Linux / MacOS
```
    mkdir work
    git clone https://github.com/herumi/cybozulib
    git clone https://github.com/herumi/msoffice
    cd msoffice
    make -j RELEASE=1
```
Windows
```
    mkdir work
    git clone https://github.com/herumi/cybozulib
    git clone https://github.com/herumi/msoffice
    git clone https://github.com/herumi/cybozulib_ext # for openssl
    cd msoffice
    mk.bat ; or open msoffice12.sln and build
```
# How to use
* Encrypt test.xlsx with a password `test`.
```
bin/msoffice-crypt.exe -e -p test test.xlsx enc.xlsx
```
* Decrypt enc.xlsx with a password `test`.
```
bin/msoffice-crypt.exe -d -p test enc.xlsx dec.xlsx
```


# Support format

Office 2010 or later Office Document format which suffix is pptx, docx, xlsx.

# DLL for Windows
* msoc.dll (Microsoft Office Crypto)

* [msoc.dll](https://github.com/herumi/msoffice/raw/master/bin/msoc.dll)
* [msoc.h](https://github.com/herumi/msoffice/blob/master/include/msoc.h)

* Encrypt `inFile` with `pass` and make `outFile`.
```
MSOC_encrypt(outFile, inFile, pass, NULL);
```
* Decrypt `inFile` with `pass` and make `outFile`.
```
MSOC_decrypt(outFile, inFile, pass, NULL);
```

# lib for Linux
* libmsoc.lib

# License
BSD 3-Clause License

Copyright (c) 2015 Cybozu Labs, Inc. All rights reserved.

# References

* Compound File Binary File Format(v20120328)
[[MS-CFB]](http://msdn.microsoft.com/en-us/library/dd942138.aspx)
* Office Document Cryptography Structure Specification(v20120412)
[[MS-OFFCRYPTO]](http://msdn.microsoft.com/en-us/library/cc313071.aspx)
* CODE BLUE 2015 [[Backdoors with the MS Office file encryption master key and a proposal for a reliable file format]](http://www.slideshare.net/herumi/backdoors-with-the-ms-office-file-encryption-master-key-and-a-proposal-for-a-reliable-file-format)

* 99.9% built by herumi <3
