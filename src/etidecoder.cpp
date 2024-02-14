#include "etidecoder.hpp"
#include "types.hpp"
#include "pickleserializer.hpp"

#include <csdr-eti/csdr-eti.hpp>

static int EtiDecoder_init(EtiDecoder* self, PyObject* args, PyObject* kwds) {
    self->inputFormat = FORMAT_COMPLEX_FLOAT;
    self->outputFormat = FORMAT_CHAR;
    self->setModule(new Csdr::Eti::EtiDecoder());

    return 0;
}

static PyObject* EtiDecoder_setMetaWriter(EtiDecoder* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "writer", NULL};

    PyTypeObject* WriterType = getWriterType();
    Writer* writer;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, WriterType, &writer)) {
        Py_DECREF(WriterType);
        return NULL;
    }
    Py_DECREF(WriterType);

    if ((PyObject*) writer != Py_None && writer->writerFormat != FORMAT_CHAR) {
        PyErr_SetString(PyExc_ValueError, "invalid writer format");
        return NULL;
    }

    if (self->metaWriter != nullptr) {
        Py_DECREF(self->metaWriter);
        self->metaWriter = nullptr;
    }

    if ((PyObject*) writer != Py_None) {
        self->metaWriter = writer;
        Py_INCREF(self->metaWriter);

        auto metaWriter = new Csdr::Eti::PipelineMetaWriter(new Csdr::Eti::PickleSerializer());
        metaWriter->setWriter(dynamic_cast<Csdr::Writer<unsigned char>*>(writer->writer));
        dynamic_cast<Csdr::Eti::EtiDecoder*>(self->module)->setMetaWriter(metaWriter);
    }

    Py_RETURN_NONE;
}

static PyObject* EtiDecoder_setServiceIdFilter(EtiDecoder* self, PyObject* args, PyObject* kwds) {
    static char* kwlist[] = {(char*) "service", NULL};
    PyObject* services_python = NULL;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &PyList_Type, &services_python)) {
        return NULL;
    }
    std::set<uint32_t> services;
    Py_ssize_t size = PyList_Size(services_python);
    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject* s = PyList_GetItem(services_python, i);
        if (s == NULL) {
            // error is already set by PyList_GetItem()
            return NULL;
        }
        if (!PyLong_Check(s)) {
            PyErr_SetString(PyExc_ValueError, "argument is not an integer");
            return NULL;
        }
        uint32_t sid = PyLong_AsUnsignedLong(s);
        if (PyErr_Occurred()) {
            return NULL;
        }
        services.insert(sid);
    }
    dynamic_cast<Csdr::Eti::EtiDecoder*>(self->module)->setServiceFilter(services);
    Py_RETURN_NONE;
}

static PyMethodDef EtiDecoderMethods[] = {
    {"setMetaWriter", (PyCFunction) EtiDecoder_setMetaWriter, METH_VARARGS | METH_KEYWORDS,
     "set writer for metadata"
    },
    {"setServiceIdFilter", (PyCFunction) EtiDecoder_setServiceIdFilter, METH_VARARGS | METH_KEYWORDS,
     "set service id filter"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot EtiDecoderSlots[] = {
    {Py_tp_init, (void*) EtiDecoder_init},
    {Py_tp_methods, EtiDecoderMethods},
    {0, 0}
};

PyType_Spec EtiDecoderSpec = {
    "csdreti.modules.EtiDecoder",
    sizeof(EtiDecoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    EtiDecoderSlots
};
