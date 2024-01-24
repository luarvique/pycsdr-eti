#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <pycsdr/module.hpp>
#include <pycsdr/writer.hpp>

struct EtiDecoder: Module {
    Writer* metaWriter;
};

extern PyType_Spec EtiDecoderSpec;