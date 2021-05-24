# ccaesarcipher

A caesar cipher program written in c. Features auotomatic decryption.

## Prerequisites

* gcc 
* make

## Build

1. `mkdir obj && mkdir bin` 
2. `cd src && make`
3. `cd ..`

## Usage

### Option 1

1. `cd bin`
2. `./caesar_cipher.exe`
3. At runtime, provide a paramters and .txt file for encryption/decryption when prompted.

### Option 2

1. `cd bin`
2. Provide paramters via command line arguments and a .txt file via input redirection (<). e.g. `\.caesar_cipher.exe decrypt auto < test.txt`.
