# CommandLinePasswordManager
CommandLine Password Manager

This program depends on boost library's `tokenizer` class to parse inputs from its shell and `Crypto++`'s `AES` implementation to encrypt the password file. The program is very primitive and does not handle any errors in input. The person who intends to use this program has to provide the right input.

There are no pre-built binaries for `Crypto++` library. Therefore one has to download the code and build either dynamic or static library from the source. I used Visual Studio 2017 for doing that. Compilation of `Crypto++` with Visual Studio requires VS2015 packages. It is a bit troublesome to get the `Crypto++` library into working.

When wrong password entered, system aborts. I need to implement exception handling.
