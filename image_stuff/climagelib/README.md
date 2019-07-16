# climglib

## Brief

This library contains some images manipulations functions running with OpenCL.

## Dependancies

To use this library, you only need `<Cl/cl.h>` and the OpenCL's drivers for your GPU. 

## Types & Structures

* `ocl_env_t`, the struct containing all OpenCL's variables.
* `img_t`, an alias for `unsigned int *`.

## Usage

0. download climglib.[hc]
1. `#include "climglib.h"`
2. create a 
2. **IMPORTANT** make sure you work with 32-bit images (RGBA) with 8-bit unsigned integers for each channel.
3. use the functions.
4. be aware that each function return a new pointer, so remind to free intermediate results.

## List of functions
