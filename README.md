# Apertium Coding Challenge

This is an attempt at the Apertium Coding Challenge for GSoC 2019.

Project: Anaphora Resolution

Author: Tanmai Khanna

## Executing the code

Anaphora Resolution where input is read byte by byte:

`python3 byte_anaphora.py < input.txt > output_byte.txt`

Anaphora Resolution where input is read as chunks:

`python3 chunk_anaphora.py 512 < input.txt > output_chunk_512.txt`

Here the chunk size you wish to use (in bytes) is given as an argument.

If you do not specify a chunk size it uses 1024 bytes as default.

`python3 chunk_anaphora.py < input.txt > output_chunk_default.txt`

## Speed-up Report

"Report_Chunk_vs_Byte.txt" shows the time taken by these different methods and compares them.

We can clearly see that chunks perform much better, and bigger the chunk size, faster the execution time.
