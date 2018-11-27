#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    Py_Initialize();

    //init stuff
    //perhaps expand the python path?
    PyRun_SimpleString(
                    "import sys\n"
                    "sys.path.append('.')\n"
                    "sys.path.append('data')\n"
    );
    // make stdout use ofLog
    /* need of binding
    PyRun_SimpleString(
                    "import sys\n"
                    "class StdoutCatcher:\n"
                    "	#this class redirect stdout to ofLog()\n"
                    "	def __init__(self):\n"
                    "		self.message = []\n"
                    "	def write(self,s):\n"
                    "		from openframeworks import ofLog, OF_LOG_NOTICE\n"
                    "		if s.endswith('\\n'):\n"
                    "			self.message.append(s[:-1])\n"
                    "			ofLog(OF_LOG_NOTICE,''.join(self.message))\n"
                    "			self.message = []\n"
                    "		else:\n"
                    "			self.message.append(s)\n"
                    "catcher = StdoutCatcher()\n"
                    "sys.stdout = catcher\n"
    );*/

    const char* args[] = {"1", "2"};
    v = {"1", "2"};//v(args, args + 2);
    //run_python( "tester", "test", v);
    init_pyfiles();
}

int ofApp::init_pyfiles()
{
    //tester.py
    PyObject *pName, *pModule;
    pName = PyUnicode_DecodeFSDefault( "tester" );
    if ( pName != NULL )
    {
        pModule = PyImport_Import( pName );
        if ( pModule != NULL )
        {
            // succeeded save to pymods map
            pymods.insert(std::pair<std::string, PyObject*>("tester", pModule));
            return 0;
        }
        else
        {
            ofLogError() << "error importing tester.py";
            return 2;
        }
    }
    else
    {
        ofLogError() << "error loading tester.py";
        return 3;
    }
}

int ofApp::run_method(PyObject* pModule, const std::string method_name)
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

int ofApp::run_python(const char* pyfile, const char* pymethod, vector<string> pyargs) {

    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;

    pName = PyUnicode_DecodeFSDefault( pyfile );
    /* Error checking of pName left out */

    ofLogNotice() << pName->ob_type->tp_name;
    pModule = PyImport_Import( pName );
    Py_DECREF(pName);


    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString( pModule, pymethod );
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check( pFunc )) {
            // arguments to the method
            pArgs = PyTuple_New(pyargs.size()); //number of arguments
            for (int i = 0; i < pyargs.size(); ++i) {
                pValue = PyLong_FromLong(atoi(pyargs[i].c_str()));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", pymethod);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pyfile);
        return 1;
    }
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 201;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofLogNotice() << run_method( pymods["tester"], "test2");
    //run_python( "tester", "test", v);
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
