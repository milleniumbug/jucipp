#!/bin/bash

function print_run {
  echo $1
  $1
}


print_run "cd jucipp" &&
print_run "mkdir -p build" &&
print_run "cd build" &&
print_run "cmake ${cmake_args} .." &&

exec "$@ ${make_args}"


