/*=================================================================
 *
 * mex_handletest.cpp
 * Test for persistence of heap-allocated objects between mex calls
 * using handles in MatLab. This technique was originally developed by 
 * Mike Stevens.
 *
 * Tim Bailey 2004
 *
 *=================================================================*/

#include "ObjectHandle.h"

class MyClass {
	int i;
public:
	MyClass() : i(0) { mexPrintf("MyClass created.\n"); }
	~MyClass() { mexPrintf("MyClass destroyed.\n"); }
	void dostuff() { mexPrintf("MyClass value %d\n", i++); }
};

void create (mxArray **out) {
#if 0 // simple interface
	*out = create_handle(new MyClass);

#else // direct interface
	MyClass *mine = new MyClass;
mexPrintf("Pointer before: %#x, ", mine);
	ObjectHandle<MyClass> *handle = new ObjectHandle<MyClass>(mine);
mexPrintf("Pointer after: %#x\n", mine);
	*out = handle->to_mex_handle();
#endif
}

void display (const mxArray *in) {
#if 1 // simple interface
	MyClass& mine = get_object<MyClass>(in);
	mine.dostuff();

#else // direct interface
	ObjectHandle<MyClass>* handle = ObjectHandle<MyClass>::from_mex_handle(in);
	handle->get_object().dostuff();
#endif
}

void destroy (const mxArray *in) {
#if 0 // simple interface
	destroy_object<MyClass>(in);

#else // direct interface
	ObjectHandle<MyClass>* handle = ObjectHandle<MyClass>::from_mex_handle(in);
	delete handle;
#endif
}

void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[] )     
{
	if (nlhs == 1 && nrhs == 0) create(&plhs[0]);
	else if ( nlhs == 0 && nrhs == 1 ) display(prhs[0]);
	else if ( nlhs == 0 && nrhs == 2 ) destroy(prhs[0]);
	else mexErrMsgTxt("Bad input.");
}
