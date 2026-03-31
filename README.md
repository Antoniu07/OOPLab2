# OOPLab2 - Caesar Cipher with Frequency Analysis

A C program that implements Caesar cipher encryption and decryption, including automatic cipher breaking using frequency analysis.

## Features

- **Caesar Cipher Encrypt/Decrypt** — shift-based encryption and decryption for alphabetic text
- **Frequency Analysis** — break unknown Caesar ciphers by comparing letter distributions against English
- **Distance Metrics** — three methods for comparing frequency histograms:
  - Chi-Squared distance
  - Euclidean distance
  - Cosine distance
- **Interactive Menu** — load text from keyboard or file, encrypt, decrypt, view histograms, and auto-break ciphers

## Additional Problems

- Maximum triplet product from an array
- Custom `strchr` implementation and character position finder
- 24 Puzzle Game (generate and solve digit/operator combinations)

## Build

```bash
mkdir build && cd build
cmake ..
make
./OOPLab2
```

## Usage

Run the program and use the menu to:
1. Load text (keyboard or file)
2. Encrypt with a chosen shift
3. Decrypt with a known shift
4. View letter frequency histogram
5. Auto-break cipher using frequency analysis