#include "types.hpp"

#include "etidecoder.hpp"

#include <csdr-eti/version.hpp>

static PyModuleDef pycsdretimodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "csdreti.modules",
    .m_doc = "Python bindings for the csdr-eti library",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_modules(void) {
    PyObject* bases = PyTuple_Pack(1, getModuleType());
    if (bases == NULL) return NULL;
    PyObject* EtiDecoderType = PyType_FromSpecWithBases(&EtiDecoderSpec, bases);
    if (EtiDecoderType == NULL) return NULL;

    PyObject *m = PyModule_Create(&pycsdretimodule);
    if (m == NULL) {
        return NULL;
    }

    PyModule_AddObject(m, "EtiDecoder", EtiDecoderType);

    PyObject* csdrEtiVersion = PyUnicode_FromStringAndSize(Csdr::Eti::version.c_str(), Csdr::Eti::version.length());
    if (csdrEtiVersion == NULL) return NULL;
    PyModule_AddObject(m, "csdreti_version", csdrEtiVersion);

    PyObject* version = PyUnicode_FromString(VERSION);
    if (version == NULL) return NULL;
    PyModule_AddObject(m, "version", version);

    return m;
}