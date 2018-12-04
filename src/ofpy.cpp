#include "ofpy.h"
#include "ofLog.h"

extern "C"{
PyObject* PyInit__openframeworks(void);
PyObject* PyInit__glm(void);
}

int init_python()
{
    PyImport_AppendInittab("_glm", PyInit__glm);
    PyImport_AppendInittab("_openframeworks", PyInit__openframeworks);
    Py_Initialize();
    // expand the python path?
    PyRun_SimpleString(
                    "import sys\n"
                    "sys.path.append('.')\n"
                    "sys.path.append('data')\n"
    );

    return 0;
}

PyObject* import_python_file( std::string filename )
{
    PyObject *pName, *pModule;
    pName = PyUnicode_DecodeFSDefault( filename.c_str() );
    if ( pName != NULL )
    {
        pModule = PyImport_Import( pName );
        if ( pModule != NULL )
        {
            // succeeded save to pymods map
            pymods.insert(std::pair<std::string, PyObject*>(filename, pModule));
            return pModule;
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            ofLogError() << "error importing " << filename;
            //return 2;
        }
    }
    else
    {
        ofLogError() << "error loading "<< filename;
        //return 3;
    }
    return NULL;
}

int run_method(PyObject* pModule, const std::string method_name)
{
    PyObject *pFunc, *pValue;
    if ( pModule != NULL )
    {
        pFunc = PyObject_GetAttrString( pModule, method_name.c_str() );
        if ( pFunc && PyCallable_Check( pFunc ) )
        {
            //we have a method
            // determine args TODO
            pValue = PyObject_CallObject(pFunc, NULL);
            if ( pValue != NULL )
            {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
                Py_DECREF(pFunc);
                return 0;
            }
            else
            {
                if (PyErr_Occurred())
                    PyErr_Print();
                ofLogError() << "Call: " << method_name << " failed";
                Py_DECREF(pFunc);
                return 1;
            }
        }
        else
        {
            if (PyErr_Occurred())
                PyErr_Print();
            ofLogError() << "Can't find method: " << method_name;
            return 2;
        }
    }
    return 3;
}
