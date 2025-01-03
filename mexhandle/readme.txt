MatLab MEX ObjectHandle<T>
--------------------------

This utility allows you to create a C++ object within a mex-file and
to pass a handle to this object back to MatLab. Thus, you can pass C++
objects between different MEX functions without first converting to
a MatLab compatible format.

Basic Usage
-----------

The ObjectHandle comes with a set of wrapper functions, which simplify
the common case.

template <typename T> 
mxArray *create_handle(T* t);

template <typename T>
T& get_object(const mxArray *mxh);

template <typename T>
void destroy_object(const mxArray *mxh);

These are demonstrated in mex_handletest.cpp.


More Advanced Usage
-------------------

To provide more control over the C++ object being handled, can use ObjectHandle<T> 
directly. These operations are also demonstrated in mex_handletest.cpp.

A particular use is to pass a static object to a handle. eg,

static MyObject obj;
ObjectHandle<MyClass> *objh = new ObjectHandle<MyObject>(obj);
mxArray *mex_handle = objh->to_mex_handle();

In the above case, since obj is passed as a reference, the ObjectHandle does 
*not* own obj, and will not automatically destroy it.

Examples
--------

1. mex_handletest.cpp

Simple example where a C++ object is handed back and forth between a single 
mex-dll. The mex-function is passed a flag to indicate what operation is to
be carried out.

If the user calls 'clear all' or 'clear mex' or 'clear functions' from within
MatLab, the C++ object is destroyed automatically, and the handle ceases to
be valid.

2. mex_create.cpp and mex_destroy.cpp

An simple example where a C++ object is created in one mex-dll and passed to
another, where it is destroyed. (See "known issues" below.) Again, the object
is destroyed automatically if the dll is cleared.


Known Issues
------------

Some memory models with MSVC++ appear to cause problems with this utility.
Specifically, to pass a handle from one MEX-dll to another, the dlls *must*
share a common address-space. Make sure that your MEX setup uses appropriate
run-time configuration to ensure shared address-space.

Note, if you are simply passing a C++ object back-and-forth between MatLab and 
a single MEX-dll, then there is no problem.
