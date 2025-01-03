/*
 */

#include "../ObjectHandle.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nlhs != 1 || nrhs != 0)
		mexErrMsgTxt("Usage: h = mex_create");

	int* obj = new int(71);
	plhs[0] = create_handle(obj);
}
