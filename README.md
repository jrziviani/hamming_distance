# Hamming distance

Implements the algorithm to calculate the Hamming distance between two binary
blobs.

## Building

```
$ git clone x
$ cd x
$ ./build.sh
```

## Testing

During the build phase, unit tests and integration tests will be executed in
order to make sure it works as intended.

## Running the client

There is a small client to calculate the Hamming distance between two strings,
it's available in the root tree after building the source code.

```
$ ./cli <string1> <string2>
$ ./cli "string with space" "string_without_space"
```

## Limitations

It was developed and tested in Linux only.
