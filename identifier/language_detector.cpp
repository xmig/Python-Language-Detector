// YAY?!?!

#include <cstdlib>
#include<string.h>
#include <Python.h>
#include "language_sample.h"
#include "language_database.h"

static PyObject *LangError;
static LanguageDatabase LangDatabase;

PyObject *lang_identify(PyObject *self, PyObject *args);
PyObject *lang_loadtrigrams(PyObject *self, PyObject *args);

static PyMethodDef LangMethods[] = {
    {"identify",  lang_identify, METH_VARARGS, "Identify a language from a unicode string."},
    {"load",  lang_loadtrigrams, METH_VARARGS, "Load the trigram files from a specified folder."},
    {NULL, NULL, 0, NULL}
};


PyObject *lang_identify(PyObject *self, PyObject *args) {
	int maxTrigrams = 300, maxInputLength = 900;
	const char *inputString;
	int stringSize; 
	if (!PyArg_ParseTuple(args, "s#ii", &inputString, &stringSize, &maxTrigrams, &maxInputLength))
		return NULL;

	LanguageSample *sample = new LanguageSample(&LangDatabase, inputString, maxTrigrams, maxInputLength);
	const char *result = sample->Identify();
	delete sample;
	
	return Py_BuildValue("s", result);
}

PyObject *lang_loadtrigrams(PyObject *self, PyObject *args) {
	const char *pathName;
	int stringSize; 
	if (!PyArg_ParseTuple(args, "s#", &pathName, &stringSize))
		return NULL;

	if (!LangDatabase.LoadAll(pathName)) {
		PyErr_SetString(LangError, "File Loads failed - Directory not found");
		return NULL;
	}
	
	return Py_BuildValue("s", pathName);
}

/* char to wchar_t */
wchar_t * ctow(const char* c, size_t max)
{
	wchar_t * w = new wchar_t[max];
	mbstowcs(w,c,max);
	return w;
}

inline void wfree( wchar_t *buff ) { delete [] buff; }


//PyMODINIT_FUNC initlanguageIdentifier(void) {
//	PyObject *m;
//	m = Py_InitModule("languageIdentifier", LangMethods);
//	if (m == NULL)
//		return;
//
//	LangError = PyErr_NewException("languageIdentifier.error", NULL, NULL);
//    Py_INCREF(LangError);
//    PyModule_AddObject(m, "error", LangError);
//}

static struct PyModuleDef languageIdentifier =
{
        PyModuleDef_HEAD_INIT,
        "languageIdentifier", /* name of module */
        "",          /* module documentation, may be NULL */
        -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
        LangMethods
};

PyMODINIT_FUNC PyInit_languageIdentifier(void)
{
    return PyModule_Create(&languageIdentifier);
}

int main (int argc, char ** argv) {
    /* Pass argv[0] to the Python interpreter */
    //Py_SetProgramName(argv[0]);

	size_t len = strlen(argv[0]);

	wchar_t* name = ctow(argv[0], len );
	name[len+1] = '\0';

    Py_SetProgramName(name);
	//wfree(name);


    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Add a static module */
    PyInit_languageIdentifier();
    //initlanguageIdentifier();
}


