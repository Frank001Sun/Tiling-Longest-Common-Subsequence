#!/bin/bash

# Check whether the number of parameters is correct
if [ "$#" -ne 2 ]; then
    echo "usage: $0 <sequence length> <file name>"
    exit 1
fi

length=$1
filename=$2

# generate the first random sequence
first_string=$(tr -dc 'a-zA-Z0-9' < /dev/urandom | head -c "$length")
echo "$first_string" > "$filename"

# generate the second random sequence and append to the file
second_string=$(tr -dc 'a-zA-Z0-9' < /dev/urandom | head -c "$length")
echo "$second_string" >> "$filename"

echo "already generate two random sequences with length $length into $filename"