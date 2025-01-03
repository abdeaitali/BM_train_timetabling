/* 
 */

#include "../ObjectHandle.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nlhs != 0 || nrhs != 1)
		mexErrMsgTxt("Usage: mex_destroy(h)");

	destroy_object<int>(prhs[0]);
}
