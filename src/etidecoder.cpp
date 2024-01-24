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

static PyMethodDef EtiDecoderMethods[] = {
    {"setMetaWriter", (PyCFunction) EtiDecoder_setMetaWriter, METH_VARARGS | METH_KEYWORDS,
     "set writer for metadata"
    },
    {NULL}  /* Sentinel */
};

static PyType_Slot EtiDecoderSlots[] = {
    {Py_tp_init, (void*) EtiDecoder_init},
    {Py_tp_methods, EtiDecoderMethods},
    {0, 0}
};

PyType_Spec EtiDecoderSpec = {
    "csdr-eti.modules.EtiDecoder",
    sizeof(EtiDecoder),
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_FINALIZE,
    EtiDecoderSlots
};
