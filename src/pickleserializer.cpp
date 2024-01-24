#include "pickleserializer.hpp"

#include <Python.h>

using namespace Csdr::Eti;

std::string PickleSerializer::serialize(std::map<std::string, std::string> metadata) {
    // acquire GIL
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject* pickle = PyImport_ImportModule("pickle");
    if (pickle == NULL) throw std::runtime_error("importing pickle module failed");

    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        Py_DECREF(pickle);
        throw std::runtime_error("creating dictionary failed");
    }

    for (auto entry: metadata) {
        PyObject* value = PyUnicode_DecodeUTF8(entry.second.c_str(), entry.second.length(), "replace");
        if (value == NULL) {
            if (PyErr_Occurred()) {
                PyErr_PrintEx(0);
            }
            continue;
        }
        if (PyDict_SetItemString(dict, entry.first.c_str(), value) == -1) {
            Py_DECREF(pickle);
            Py_DECREF(dict);
            Py_DECREF(value);
            throw std::runtime_error("failed to put item into dictionary");
        }

        Py_DECREF(value);
    }

    PyObject* result = PyObject_CallMethod(pickle, "dumps", "O", dict);
    if (result == NULL) {
        Py_DECREF(pickle);
        Py_DECREF(dict);
        throw std::runtime_error("failed to pickle dictionary");
    }

    Py_DECREF(dict);
    Py_DECREF(pickle);

    if (!PyBytes_Check(result)) {
        Py_DECREF(result);
        throw std::runtime_error("unexpected result (was expecting bytes)");
    }

    char* bytes;
    Py_ssize_t length;
    if (PyBytes_AsStringAndSize(result, &bytes, &length) == -1) {
        Py_DECREF(result);
        throw std::runtime_error("failed to extract result bytes");
    }

    std::string serialized(bytes, length);
    Py_DECREF(result);

    /* Release the thread. No Python API allowed beyond this point. */
    PyGILState_Release(gstate);

    return serialized;
}

std::string PickleSerializer::serializeProgrammes(std::map <uint16_t, std::string> metadata) {
    // acquire GIL
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject* pickle = PyImport_ImportModule("pickle");
    if (pickle == NULL) throw std::runtime_error("importing pickle module failed");

    PyObject* programmeDict = PyDict_New();
    if (programmeDict == NULL) {
        Py_DECREF(pickle);
        throw std::runtime_error("creating dictionary failed");
    }

    for (auto entry: metadata) {
        PyObject* value = PyUnicode_DecodeUTF8(entry.second.c_str(), entry.second.length(), "replace");
        if (value == NULL) {
            if (PyErr_Occurred()) {
                PyErr_PrintEx(0);
            }
            continue;
        }

        PyObject* key = PyLong_FromUnsignedLong(entry.first);
        if (key == NULL) {
            Py_DECREF(value);
            if (PyErr_Occurred()) {
                PyErr_PrintEx(0);
            }
            continue;
        }

        if (PyDict_SetItem(programmeDict, key, value) == -1) {
            Py_DECREF(pickle);
            Py_DECREF(programmeDict);
            Py_DECREF(value);
            throw std::runtime_error("failed to put item into dictionary");
        }

        Py_DECREF(value);
    }

    PyObject* dict = PyDict_New();
    if (dict == NULL) {
        Py_DECREF(pickle);
        Py_DECREF(programmeDict);
        throw std::runtime_error("creating dictionary failed");
    }

    if (PyDict_SetItemString(dict, "programmes", programmeDict) == -1) {
        Py_DECREF(pickle);
        Py_DECREF(programmeDict);
        Py_DECREF(dict);
        throw std::runtime_error("could not set dictionary key");
    }

    PyObject* result = PyObject_CallMethod(pickle, "dumps", "O", dict);
    if (result == NULL) {
        Py_DECREF(pickle);
        Py_DECREF(dict);
        throw std::runtime_error("failed to pickle dictionary");
    }

    Py_DECREF(dict);
    Py_DECREF(pickle);

    if (!PyBytes_Check(result)) {
        Py_DECREF(result);
        throw std::runtime_error("unexpected result (was expecting bytes)");
    }

    char* bytes;
    Py_ssize_t length;
    if (PyBytes_AsStringAndSize(result, &bytes, &length) == -1) {
        Py_DECREF(result);
        throw std::runtime_error("failed to extract result bytes");
    }

    std::string serialized(bytes, length);
    Py_DECREF(result);

    /* Release the thread. No Python API allowed beyond this point. */
    PyGILState_Release(gstate);

    return serialized;
}