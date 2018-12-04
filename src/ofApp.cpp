#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    init_python();
    pymods["glm"] = import_python_file("glm");
    pymods["openframeworks"] = import_python_file("openframeworks");
    pymods["tester"] = import_python_file("tester");
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
    return 201;
}

//--------------------------------------------------------------
void ofApp::update(){
    //ofLogNotice() << run_method( pymods["tester"], "test2");
    //run_python( "tester", "test", v);
}

//--------------------------------------------------------------
void ofApp::draw(){
    run_method( pymods["tester"], "draw" );
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

void ofApp::exit()
{
    if (Py_FinalizeEx() < 0) {
        ofLogError() << "Some error while finalizing Python";
    }
}
