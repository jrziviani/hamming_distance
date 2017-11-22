# Hamming distance

Implements the algorithm to calculate the Hamming distance between two binary
blobs.

## Building

This project requires Linux, g++, and cmake.

```
$ git clone https://github.com/jrziviani/hamming_distance.git
$ cd hamming_distance
$ ./build.sh
```

## Testing

During the build phase, unit tests and integration tests will be executed in
order to make sure it works as intended.

```
% ./build.sh
[Building]-----------------------------
<snip>

[Running unit tests]-------------------
[ OK ] 10000 test cases run successfuly.
[ OK ] 4 test cases run successfuly.
[ OK ] hamming distances takes 23.4136 microseconds to run the whole test case
file
[ OK ] loop_hamming distances takes 114.795 microseconds to run the whole test
case file
[ OK ] first algorithm version is 79.6039% faster
[unit test finished]-------------------

[Running integration tests]------------
[ OK ] test empty string
[ OK ] test null string
[ OK ] 1 - test different size string
[ OK ] 2 - test different size string
[ OK ] 3 - test different size string
[ OK ] 20 test string
[integration test finished]------------
```

## Running the client

There is a small client to calculate the Hamming distance between two strings,
it's available in the root tree after building the source code.

```
$ ./cli <string1> <string2>
$ ./cli "string with space" "string_without_space"
```

## Limitations

It was developed and tested in Linux only.
