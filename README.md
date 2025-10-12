# Compression Utility using Huffman & LZW
This command-line tool is designed to compress and decompress files using two classic lossless compression algorithms: **Huffman Coding and Lempel-Ziv-Welch (LZW)**. The primary goal is to provide a flexible, algorithm-selectable utility that reduces file size while ensuring that all original data is perfectly preserved upon decompression.

The utility can process any file type, but compression effectiveness will vary based on the file's content and the chosen algorithm. Compressed files are saved with a .groza extension.

## Key Features

 - Multi-file Processing: You can compress or decompress multiple files in a single command.
 - Algorithm Selection: Easily choose between Huffman and LZW compression to suit your needs.
 - Lossless Compression: Guarantees that the decompressed file is identical to the original, with no data loss.
 - Command-Line Interface (CLI): A simple and efficient interface for all operations.

## Build Instructions
First, compile the C++ source files using a standard compiler like g++.

```g++ *.cpp -o compression_tool -std=c++17```

## Usage
The program follows a simple command-line structure. You need to specify the input file(s) and the desired compression method.

### Compression

To compress one or more files, list the file paths and add the algorithm flag (-h for Huffman, -l for LZW).

```./compression_tool <file1> [file2...] <algorithm>```

**Examples:**
 - Compressing a text file using Huffman:

   ``` ./compression_tool my_document.txt -h```

  This will create my_document.groza.
 - Compressing multiple files using LZW:

    ```./compression_tool image.jpg data.csv -l```

  This will create image.groza and data.groza.

### Decompression

The tool automatically detects if a file has the .groza extension and performs decompression. You still need to specify the algorithm that was originally used for compression.

```./compression_tool <file.groza> <algorithm>```

**Example:**

Decompressing a file that was compressed with Huffman:
```./compression_tool my_document.groza -h```
This will restore the original my_document.txt.

## Huffman compression results

| File name               | File size | Compression time | Compressed file size | Compression ratio | Decompression time | Lossless decompression |
|-------------------------|-----------|------------------|----------------------|-------------------|--------------------|------------------------|
| alice in wonderland.txt | 149 KB    | Less than 1s     | 85 KB                | 57.04%            | Less than 1s       | Yes                    |
| war and peace.txt       | 3.2 MB    | 3.83s            | 1.8 MB               | 56.25%            | 4s                 | Yes                    |
| 1.jpg                   | 663 KB    | 1.35s            | 657 KB               | 99.09%            | Less than 1s       | Yes                    |
| 1.png                   | 2 MB      | 3.65s            | 2 MB                 | 100%              | 3.16s              | Yes                    |
| progit.pdf              | 18.8 MB   | 32.29s           | 17.6 MB              | 93.61%            | 28.97s             | Yes                    |
| numbers.bin             | 80 MB     | 138s             | 69.8 MB              | 87.25%            | 118s               | Yes                    |
| test icon.svg           | 4.8 MB    | 7.23s            | 3.6 MB               | 75%               | 6.50s              | Yes                    |


## LZW compression results
| File name               | File size | Compression time | Compressed file size | Compression ratio | Decompression time | Lossless decompression |
|-------------------------|-----------|------------------|----------------------|-------------------|--------------------|------------------------|
| alice in wonderland.txt | 149 KB    | Less than 1s     | 81 KB                | 54.36%            | Less than 1s       | Yes                    |
| war and peace.txt       | 3.2 MB    | 5.82s            | 1.8 MB               | 56.25%            | 1.35s              | Yes                    |
| 1.jpg                   | 663 KB    | 2.41s            | 915 KB               | 138%              | Less than 1s       | Yes                    |
| 1.png                   | 2 MB      | 6.92s            | 2.8 MB               | 140%              | 1.98s              | No                     |
| progit.pdf              | 18.8 MB   | 51.92s           | 19.3 MB              | 102.65%           | 12.47s             | Yes                    |
| numbers.bin             | 80 MB     | 180s             | 60.1 MB              | 75.13%            | 38.29s             | Yes                    |
| test icon.svg           | 4.8 MB    | 14s              | 5.2 MB               | 108.30%           | 3.35s              | Yes                    |


