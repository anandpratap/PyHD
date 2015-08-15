/*
 * Hyperdual type for NumPy, developed from https://github.com/numpy/numpy-dtypes  Quaternion type for NumPy  Copyright (c) 2011 Martin Ling
 * Copyright (c) 2015 Anand Pratap Singh
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the NumPy Developers nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define NPY_NO_DEPRECATED_API NPY_API_VERSION

#include <Python.h>
#include <numpy/arrayobject.h>
#include <numpy/npy_math.h>
#include <numpy/ufuncobject.h>
#include "structmember.h"
#include "numpy/npy_3kcompat.h"

#include "hyperdual.h"

typedef struct {
        PyObject_HEAD
        hyperdual obval;
} PyHyperdualScalarObject;

PyMemberDef PyHyperdualArrType_members[] = {
    {"real", T_DOUBLE, offsetof(PyHyperdualScalarObject, obval.f0), READONLY,
        "The real component of the hyperdual"},
    {"f0", T_DOUBLE, offsetof(PyHyperdualScalarObject, obval.f0), READONLY,
        "The real imaginaryof the hyperdual"},
    {"f1", T_DOUBLE, offsetof(PyHyperdualScalarObject, obval.f1), READONLY,
        "The first imaginary component of the hyperdual"},
    {"f2", T_DOUBLE, offsetof(PyHyperdualScalarObject, obval.f2), READONLY,
        "The second imaginary component of the hyperdual"},
    {"f12", T_DOUBLE, offsetof(PyHyperdualScalarObject, obval.f12), READONLY,
        "The mixed imaginary component of the hyperdual"},
    {NULL}
};

static PyObject *
PyHyperdualArrType_get_components(PyObject *self, void *closure)
{
    hyperdual *q = &((PyHyperdualScalarObject *)self)->obval;
    PyObject *tuple = PyTuple_New(4);
    PyTuple_SET_ITEM(tuple, 0, PyFloat_FromDouble(q->f0));
    PyTuple_SET_ITEM(tuple, 1, PyFloat_FromDouble(q->f1));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble(q->f2));
    PyTuple_SET_ITEM(tuple, 3, PyFloat_FromDouble(q->f12));
    return tuple;
}

static PyObject *
PyHyperdualArrType_get_imag(PyObject *self, void *closure)
{
    hyperdual *q = &((PyHyperdualScalarObject *)self)->obval;
    PyObject *tuple = PyTuple_New(3);
    PyTuple_SET_ITEM(tuple, 0, PyFloat_FromDouble(q->f1));
    PyTuple_SET_ITEM(tuple, 1, PyFloat_FromDouble(q->f2));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble(q->f12));
    return tuple;
}

PyGetSetDef PyHyperdualArrType_getset[] = {
    {"components", PyHyperdualArrType_get_components, NULL,
        "The components of the hyperdual as a (f0,f1,f2,f12) tuple", NULL},
    {"imag", PyHyperdualArrType_get_imag, NULL,
        "The imaginary part of the hyperdual as an (f1,f2,f12) tuple", NULL},
    {NULL}
};

PyTypeObject PyHyperdualArrType_Type = {
#if defined(NPY_PY3K)
    PyVarObject_HEAD_INIT(NULL, 0)
#else
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
#endif
    "hyperdual.hyperdual",                    /* tp_name*/
    sizeof(PyHyperdualScalarObject),           /* tp_basicsize*/
    0,                                          /* tp_itemsize */
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
#if defined(NPY_PY3K)
    0,                                          /* tp_reserved */
#else
    0,                                          /* tp_compare */
#endif
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    0,                                          /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    PyHyperdualArrType_members,                /* tp_members */
    PyHyperdualArrType_getset,                 /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    0,                                          /* tp_new */
    0,                                          /* tp_free */
    0,                                          /* tp_is_gc */
    0,                                          /* tp_bases */
    0,                                          /* tp_mro */
    0,                                          /* tp_cache */
    0,                                          /* tp_subclasses */
    0,                                          /* tp_weaklist */
    0,                                          /* tp_del */
#if PY_VERSION_HEX >= 0x02060000
    0,                                          /* tp_version_tag */
#endif
};

static PyArray_ArrFuncs _PyHyperdual_ArrFuncs;
PyArray_Descr *hyperdual_descr;

static PyObject *
HYPERDUAL_getitem(char *ip, PyArrayObject *ap)
{
    hyperdual q;
    PyObject *tuple;

    if ((ap == NULL) || PyArray_ISBEHAVED_RO(ap)) {
        q = *((hyperdual *)ip);
    }
    else {
        PyArray_Descr *descr;
        descr = PyArray_DescrFromType(NPY_DOUBLE);
        descr->f->copyswap(&q.f0, ip, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f1, ip+8, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f2, ip+16, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f12, ip+24, !PyArray_ISNOTSWAPPED(ap), NULL);
        Py_DECREF(descr);
    }

    tuple = PyTuple_New(4);
    PyTuple_SET_ITEM(tuple, 0, PyFloat_FromDouble(q.f0));
    PyTuple_SET_ITEM(tuple, 1, PyFloat_FromDouble(q.f1));
    PyTuple_SET_ITEM(tuple, 2, PyFloat_FromDouble(q.f2));
    PyTuple_SET_ITEM(tuple, 3, PyFloat_FromDouble(q.f12));

    return tuple;
}

static int HYPERDUAL_setitem(PyObject *op, char *ov, PyArrayObject *ap)
{
    hyperdual q;

    if (PyArray_IsScalar(op, Hyperdual)) {
        q = ((PyHyperdualScalarObject *)op)->obval;
    }
    else {
        q.f0 = PyFloat_AsDouble(PyTuple_GetItem(op, 0));
        q.f1 = PyFloat_AsDouble(PyTuple_GetItem(op, 1));
        q.f2 = PyFloat_AsDouble(PyTuple_GetItem(op, 2));
        q.f12 = PyFloat_AsDouble(PyTuple_GetItem(op, 3));
    }
    if (PyErr_Occurred()) {
        if (PySequence_Check(op)) {
            PyErr_Clear();
            PyErr_SetString(PyExc_ValueError,
                    "setting an array element with a sequence.");
        }
        return -1;
    }
    if (ap == NULL || PyArray_ISBEHAVED(ap))
        *((hyperdual *)ov)=q;
    else {
        PyArray_Descr *descr;
        descr = PyArray_DescrFromType(NPY_DOUBLE);
        descr->f->copyswap(ov, &q.f0, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(ov+8, &q.f1, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(ov+16, &q.f2, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(ov+24, &q.f12, !PyArray_ISNOTSWAPPED(ap), NULL);
        Py_DECREF(descr);
    }

    return 0;
}

static void
HYPERDUAL_copyswap(hyperdual *dst, hyperdual *src,
        int swap, void *NPY_UNUSED(arr))
{
    PyArray_Descr *descr;
    descr = PyArray_DescrFromType(NPY_DOUBLE);
    descr->f->copyswapn(dst, sizeof(double), src, sizeof(double), 4, swap, NULL);
    Py_DECREF(descr);
}

static void
HYPERDUAL_copyswapn(hyperdual *dst, npy_intp dstride,
        hyperdual *src, npy_intp sstride,
        npy_intp n, int swap, void *NPY_UNUSED(arr))
{
    PyArray_Descr *descr;
    descr = PyArray_DescrFromType(NPY_DOUBLE);
    descr->f->copyswapn(&dst->f0, dstride, &src->f0, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->f1, dstride, &src->f1, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->f2, dstride, &src->f2, sstride, n, swap, NULL);
    descr->f->copyswapn(&dst->f12, dstride, &src->f12, sstride, n, swap, NULL);
    Py_DECREF(descr);    
}

static int
HYPERDUAL_compare (hyperdual *pa, hyperdual *pb, PyArrayObject *NPY_UNUSED(ap))
{
    hyperdual a = *pa, b = *pb;
    npy_bool anan, bnan;
    int ret;

    anan = hyperdual_isnan(a);
    bnan = hyperdual_isnan(b);

    if (anan) {
        ret = bnan ? 0 : -1;
    } else if (bnan) {
        ret = 1;
    } else if(hyperdual_less(a, b)) {
        ret = -1;
    } else if(hyperdual_less(b, a)) {
        ret = 1;
    } else {
        ret = 0;
    }
    printf("compare..............");
    return ret;
}

static int
HYPERDUAL_argmax(hyperdual *ip, npy_intp n, npy_intp *max_ind, PyArrayObject *NPY_UNUSED(aip))
{
    npy_intp i;
    hyperdual mp = *ip;

    *max_ind = 0;

    if (hyperdual_isnan(mp)) {
        /* nan encountered; it's maximal */
        return 0;
    }

    for (i = 1; i < n; i++) {
        ip++;
        /*
         * Propagate nans, similarly as max() and min()
         */
        if (!(hyperdual_less_equal(*ip, mp))) {  /* negated, for correct nan handling */
            mp = *ip;
            *max_ind = i;
            if (hyperdual_isnan(mp)) {
                /* nan encountered, it's maximal */
                break;
            }
        }
    }
    return 0;
}

static npy_bool
HYPERDUAL_nonzero (char *ip, PyArrayObject *ap)
{
    hyperdual q;
    if (ap == NULL || PyArray_ISBEHAVED_RO(ap)) {
        q = *(hyperdual *)ip;
    }
    else {
        PyArray_Descr *descr;
        descr = PyArray_DescrFromType(NPY_DOUBLE);
        descr->f->copyswap(&q.f0, ip, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f1, ip+8, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f2, ip+16, !PyArray_ISNOTSWAPPED(ap), NULL);
        descr->f->copyswap(&q.f12, ip+24, !PyArray_ISNOTSWAPPED(ap), NULL);
        Py_DECREF(descr);
    }
    return (npy_bool) !hyperdual_equal(q, (hyperdual) {0,0,0,0});
}

static void
HYPERDUAL_fillwithscalar(hyperdual *buffer, npy_intp length, hyperdual *value, void *NPY_UNUSED(ignored))
{
    npy_intp i;
    hyperdual val = *value;

    for (i = 0; i < length; ++i) {
        buffer[i] = val;
    }
}

#define MAKE_T_TO_HYPERDUAL(TYPE, type)                                       \
static void                                                                    \
TYPE ## _to_hyperdual(type *ip, hyperdual *op, npy_intp n,                   \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    while (n--) {                                                              \
        op->f0 = (double)(*ip++);                                               \
        op->f1 = 0;                                                             \
        op->f2 = 0;                                                             \
        op->f12 = 0;                                                             \
    }                                                                          \
}

MAKE_T_TO_HYPERDUAL(FLOAT, npy_float);
MAKE_T_TO_HYPERDUAL(DOUBLE, npy_double);
MAKE_T_TO_HYPERDUAL(LONGDOUBLE, npy_longdouble);
MAKE_T_TO_HYPERDUAL(BOOL, npy_bool);
MAKE_T_TO_HYPERDUAL(BYTE, npy_byte);
MAKE_T_TO_HYPERDUAL(UBYTE, npy_ubyte);
MAKE_T_TO_HYPERDUAL(SHORT, npy_short);
MAKE_T_TO_HYPERDUAL(USHORT, npy_ushort);
MAKE_T_TO_HYPERDUAL(INT, npy_int);
MAKE_T_TO_HYPERDUAL(UINT, npy_uint);
MAKE_T_TO_HYPERDUAL(LONG, npy_long);
MAKE_T_TO_HYPERDUAL(ULONG, npy_ulong);
MAKE_T_TO_HYPERDUAL(LONGLONG, npy_longlong);
MAKE_T_TO_HYPERDUAL(ULONGLONG, npy_ulonglong);

#define MAKE_CT_TO_HYPERDUAL(TYPE, type)                                      \
static void                                                                    \
TYPE ## _to_hyperdual(type *ip, hyperdual *op, npy_intp n,                   \
               PyArrayObject *NPY_UNUSED(aip), PyArrayObject *NPY_UNUSED(aop)) \
{                                                                              \
    while (n--) {                                                              \
        op->f0 = (double)(*ip++);                                               \
        op->f1 = (double)(*ip++);                                               \
        op->f2 = 0;                                                             \
        op->f12 = 0;                                                             \
    }                                                                          \
}

MAKE_CT_TO_HYPERDUAL(CFLOAT, npy_float);
MAKE_CT_TO_HYPERDUAL(CDOUBLE, npy_double);
MAKE_CT_TO_HYPERDUAL(CLONGDOUBLE, npy_longdouble);

static void register_cast_function(int sourceType, int destType, PyArray_VectorUnaryFunc *castfunc)
{
    PyArray_Descr *descr = PyArray_DescrFromType(sourceType);
    PyArray_RegisterCastFunc(descr, destType, castfunc);
    PyArray_RegisterCanCast(descr, destType, NPY_NOSCALAR);
    Py_DECREF(descr);
}

static PyObject *
hyperdual_arrtype_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    hyperdual q;

    if (!PyArg_ParseTuple(args, "dddd", &q.f0, &q.f1, &q.f2, &q.f12))
        return NULL;

    return PyArray_Scalar(&q, hyperdual_descr, NULL);
}

static PyObject *
gentype_richcompare(PyObject *self, PyObject *other, int cmp_op)
{
    PyObject *arr, *ret;

    arr = PyArray_FromScalar(self, NULL);
    if (arr == NULL) {
        return NULL;
    }
    ret = Py_TYPE(arr)->tp_richcompare(arr, other, cmp_op);
    Py_DECREF(arr);
    return ret;
}

static long
hyperdual_arrtype_hash(PyObject *o)
{
    hyperdual q = ((PyHyperdualScalarObject *)o)->obval;
    long value = 0x456789;
    value = (10000004 * value) ^ _Py_HashDouble(q.f0);
    value = (10000004 * value) ^ _Py_HashDouble(q.f1);
    value = (10000004 * value) ^ _Py_HashDouble(q.f2);
    value = (10000004 * value) ^ _Py_HashDouble(q.f12);
    if (value == -1)
        value = -2;
    return value;
}

static PyObject *
hyperdual_arrtype_repr(PyObject *o)
{
    char str[128];
    hyperdual q = ((PyHyperdualScalarObject *)o)->obval;
    sprintf(str, "hyperdual(%g, %g, %g, %g)", q.f0, q.f1, q.f2, q.f12);
    return PyUString_FromString(str);
}

static PyObject *
hyperdual_arrtype_str(PyObject *o)
{
    char str[128];
    hyperdual q = ((PyHyperdualScalarObject *)o)->obval;
    sprintf(str, "hyperdual(%g, %g, %g, %g)", q.f0, q.f1, q.f2, q.f12);
    return PyString_FromString(str);
}

static PyMethodDef HyperdualMethods[] = {
    {NULL, NULL, 0, NULL}
};

#define UNARY_UFUNC(name, ret_type)\
static void \
hyperdual_##name##_ufunc(char** args, npy_intp* dimensions,\
    npy_intp* steps, void* data) {\
    char *ip1 = args[0], *op1 = args[1];\
    npy_intp is1 = steps[0], os1 = steps[1];\
    npy_intp n = dimensions[0];\
    npy_intp i;\
    for(i = 0; i < n; i++, ip1 += is1, op1 += os1){\
        const hyperdual in1 = *(hyperdual *)ip1;\
        *((ret_type *)op1) = hyperdual_##name(in1);};}

UNARY_UFUNC(isnan, npy_bool)
UNARY_UFUNC(isinf, npy_bool)
UNARY_UFUNC(isfinite, npy_bool)
UNARY_UFUNC(absolute, hyperdual)
UNARY_UFUNC(log, hyperdual)
UNARY_UFUNC(exp, hyperdual)
UNARY_UFUNC(sin, hyperdual)
UNARY_UFUNC(cos, hyperdual)
UNARY_UFUNC(tan, hyperdual)
UNARY_UFUNC(arcsin, hyperdual)
UNARY_UFUNC(arccos, hyperdual)
UNARY_UFUNC(arctan, hyperdual)
UNARY_UFUNC(sqrt, hyperdual)



UNARY_UFUNC(negative, hyperdual)
UNARY_UFUNC(conjugate, hyperdual)

#define BINARY_GEN_UFUNC(name, func_name, arg_type, ret_type)\
static void \
hyperdual_##func_name##_ufunc(char** args, npy_intp* dimensions,\
    npy_intp* steps, void* data) {\
    char *ip1 = args[0], *ip2 = args[1], *op1 = args[2];\
    npy_intp is1 = steps[0], is2 = steps[1], os1 = steps[2];\
    npy_intp n = dimensions[0];\
    npy_intp i;\
    for(i = 0; i < n; i++, ip1 += is1, ip2 += is2, op1 += os1){\
        const hyperdual in1 = *(hyperdual *)ip1;\
        const arg_type in2 = *(arg_type *)ip2;\
        *((ret_type *)op1) = hyperdual_##func_name(in1, in2);};};

#define BINARY_UFUNC(name, ret_type)\
    BINARY_GEN_UFUNC(name, name, hyperdual, ret_type)
#define BINARY_SCALAR_UFUNC(name, ret_type)\
    BINARY_GEN_UFUNC(name, name##_scalar, npy_double, ret_type)

BINARY_UFUNC(add, hyperdual)
BINARY_UFUNC(subtract, hyperdual)
BINARY_UFUNC(multiply, hyperdual)
BINARY_UFUNC(divide, hyperdual)
BINARY_UFUNC(power, hyperdual)
BINARY_UFUNC(copysign, hyperdual)
BINARY_UFUNC(equal, npy_bool)
BINARY_UFUNC(not_equal, npy_bool)
BINARY_UFUNC(less, npy_bool)
BINARY_UFUNC(less_equal, npy_bool)
BINARY_UFUNC(greater, npy_bool)
BINARY_UFUNC(greater_equal, npy_bool)


BINARY_SCALAR_UFUNC(add, hyperdual)
BINARY_SCALAR_UFUNC(subtract, hyperdual)

BINARY_SCALAR_UFUNC(multiply, hyperdual)
BINARY_SCALAR_UFUNC(divide, hyperdual)
BINARY_SCALAR_UFUNC(power, hyperdual)

BINARY_SCALAR_UFUNC(less_equal, npy_bool)
BINARY_SCALAR_UFUNC(less, npy_bool)
BINARY_SCALAR_UFUNC(greater_equal, npy_bool)
BINARY_SCALAR_UFUNC(greater, npy_bool)

BINARY_SCALAR_UFUNC(equal, npy_bool)
BINARY_SCALAR_UFUNC(not_equal, npy_bool)


#if defined(NPY_PY3K)
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "numpy_hyperdual",
    NULL,
    -1,
    HyperdualMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
#endif

#if defined(NPY_PY3K)
PyMODINIT_FUNC PyInit_numpy_hyperdual(void) {
#else
PyMODINIT_FUNC initnumpy_hyperdual(void) {
#endif

    PyObject *m;
    int hyperdualNum;
    PyObject* numpy = PyImport_ImportModule("numpy");
    PyObject* numpy_dict = PyModule_GetDict(numpy);
    int arg_types[3];

#if defined(NPY_PY3K)
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("numpy_hyperdual", HyperdualMethods);
#endif

    if (!m) {
        return NULL;
    }

    /* Make sure NumPy is initialized */
    import_array();
    import_umath();

    /* Register the hyperdual array scalar type */
#if defined(NPY_PY3K)
    PyHyperdualArrType_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
#else
    PyHyperdualArrType_Type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_CHECKTYPES;
#endif
    PyHyperdualArrType_Type.tp_new = hyperdual_arrtype_new;
    PyHyperdualArrType_Type.tp_richcompare = gentype_richcompare;
    PyHyperdualArrType_Type.tp_hash = hyperdual_arrtype_hash;
    PyHyperdualArrType_Type.tp_repr = hyperdual_arrtype_repr;
    PyHyperdualArrType_Type.tp_str = hyperdual_arrtype_str;
    PyHyperdualArrType_Type.tp_base = &PyGenericArrType_Type;
    if (PyType_Ready(&PyHyperdualArrType_Type) < 0) {
        PyErr_Print();
        PyErr_SetString(PyExc_SystemError, "could not initialize PyHyperdualArrType_Type");
        return NULL;
    }

    /* The array functions */
    PyArray_InitArrFuncs(&_PyHyperdual_ArrFuncs);
    _PyHyperdual_ArrFuncs.getitem = (PyArray_GetItemFunc*)HYPERDUAL_getitem;
    _PyHyperdual_ArrFuncs.setitem = (PyArray_SetItemFunc*)HYPERDUAL_setitem;
    _PyHyperdual_ArrFuncs.copyswap = (PyArray_CopySwapFunc*)HYPERDUAL_copyswap;
    _PyHyperdual_ArrFuncs.copyswapn = (PyArray_CopySwapNFunc*)HYPERDUAL_copyswapn;
    _PyHyperdual_ArrFuncs.compare = (PyArray_CompareFunc*)HYPERDUAL_compare;
    _PyHyperdual_ArrFuncs.argmax = (PyArray_ArgFunc*)HYPERDUAL_argmax;
    _PyHyperdual_ArrFuncs.nonzero = (PyArray_NonzeroFunc*)HYPERDUAL_nonzero;
    _PyHyperdual_ArrFuncs.fillwithscalar = (PyArray_FillWithScalarFunc*)HYPERDUAL_fillwithscalar;

    /* The hyperdual array descr */
    hyperdual_descr = PyObject_New(PyArray_Descr, &PyArrayDescr_Type);
    hyperdual_descr->typeobj = &PyHyperdualArrType_Type;
    hyperdual_descr->kind = 'q';
    hyperdual_descr->type = 'j';
    hyperdual_descr->byteorder = '=';
    hyperdual_descr->type_num = 0; /* assigned at registration */
    hyperdual_descr->elsize = 8*4;
    hyperdual_descr->alignment = 8;
    hyperdual_descr->subarray = NULL;
    hyperdual_descr->fields = NULL;
    hyperdual_descr->names = NULL;
    hyperdual_descr->f = &_PyHyperdual_ArrFuncs;

    Py_INCREF(&PyHyperdualArrType_Type);
    hyperdualNum = PyArray_RegisterDataType(hyperdual_descr);

    if (hyperdualNum < 0)
        return NULL;

    register_cast_function(NPY_BOOL, hyperdualNum, (PyArray_VectorUnaryFunc*)BOOL_to_hyperdual);
    register_cast_function(NPY_BYTE, hyperdualNum, (PyArray_VectorUnaryFunc*)BYTE_to_hyperdual);
    register_cast_function(NPY_UBYTE, hyperdualNum, (PyArray_VectorUnaryFunc*)UBYTE_to_hyperdual);
    register_cast_function(NPY_SHORT, hyperdualNum, (PyArray_VectorUnaryFunc*)SHORT_to_hyperdual);
    register_cast_function(NPY_USHORT, hyperdualNum, (PyArray_VectorUnaryFunc*)USHORT_to_hyperdual);
    register_cast_function(NPY_INT, hyperdualNum, (PyArray_VectorUnaryFunc*)INT_to_hyperdual);
    register_cast_function(NPY_UINT, hyperdualNum, (PyArray_VectorUnaryFunc*)UINT_to_hyperdual);
    register_cast_function(NPY_LONG, hyperdualNum, (PyArray_VectorUnaryFunc*)LONG_to_hyperdual);
    register_cast_function(NPY_ULONG, hyperdualNum, (PyArray_VectorUnaryFunc*)ULONG_to_hyperdual);
    register_cast_function(NPY_LONGLONG, hyperdualNum, (PyArray_VectorUnaryFunc*)LONGLONG_to_hyperdual);
    register_cast_function(NPY_ULONGLONG, hyperdualNum, (PyArray_VectorUnaryFunc*)ULONGLONG_to_hyperdual);
    register_cast_function(NPY_FLOAT, hyperdualNum, (PyArray_VectorUnaryFunc*)FLOAT_to_hyperdual);
    register_cast_function(NPY_DOUBLE, hyperdualNum, (PyArray_VectorUnaryFunc*)DOUBLE_to_hyperdual);
    register_cast_function(NPY_LONGDOUBLE, hyperdualNum, (PyArray_VectorUnaryFunc*)LONGDOUBLE_to_hyperdual);
    register_cast_function(NPY_CFLOAT, hyperdualNum, (PyArray_VectorUnaryFunc*)CFLOAT_to_hyperdual);
    register_cast_function(NPY_CDOUBLE, hyperdualNum, (PyArray_VectorUnaryFunc*)CDOUBLE_to_hyperdual);
    register_cast_function(NPY_CLONGDOUBLE, hyperdualNum, (PyArray_VectorUnaryFunc*)CLONGDOUBLE_to_hyperdual);

#define REGISTER_UFUNC(name)\
    PyUFunc_RegisterLoopForType((PyUFuncObject *)PyDict_GetItemString(numpy_dict, #name),\
            hyperdual_descr->type_num, hyperdual_##name##_ufunc, arg_types, NULL)

#define REGISTER_SCALAR_UFUNC(name)\
    PyUFunc_RegisterLoopForType((PyUFuncObject *)PyDict_GetItemString(numpy_dict, #name),\
            hyperdual_descr->type_num, hyperdual_##name##_scalar_ufunc, arg_types, NULL)

    /* quat -> bool */
    arg_types[0] = hyperdual_descr->type_num;
    arg_types[1] = NPY_BOOL;

    REGISTER_UFUNC(isnan);
    REGISTER_UFUNC(isinf);
    REGISTER_UFUNC(isfinite);
    /* quat -> double */
    arg_types[1] = NPY_DOUBLE;

    /* quat -> quat */
    arg_types[1] = hyperdual_descr->type_num;

    REGISTER_UFUNC(sin);
    REGISTER_UFUNC(cos);
    REGISTER_UFUNC(tan);
    REGISTER_UFUNC(arcsin);
    REGISTER_UFUNC(arccos);
    REGISTER_UFUNC(arctan);
    REGISTER_UFUNC(sqrt);
    REGISTER_UFUNC(log);
    REGISTER_UFUNC(exp);
    REGISTER_UFUNC(negative);
    REGISTER_UFUNC(conjugate);
    REGISTER_UFUNC(absolute);
	
    

    /* quat, quat -> bool */

    arg_types[2] = NPY_BOOL;

    REGISTER_UFUNC(equal);
    REGISTER_UFUNC(not_equal);
    REGISTER_UFUNC(less);
    REGISTER_UFUNC(less_equal);
    REGISTER_UFUNC(greater);
    REGISTER_UFUNC(greater_equal);

    REGISTER_SCALAR_UFUNC(not_equal);
    REGISTER_SCALAR_UFUNC(equal);
    REGISTER_SCALAR_UFUNC(less);
    REGISTER_SCALAR_UFUNC(less_equal);
    REGISTER_SCALAR_UFUNC(greater);
    REGISTER_SCALAR_UFUNC(greater_equal);


    /* quat, double -> quat */

    arg_types[1] = NPY_DOUBLE;
    arg_types[2] = hyperdual_descr->type_num;

    REGISTER_SCALAR_UFUNC(multiply);
    REGISTER_SCALAR_UFUNC(divide);
    REGISTER_SCALAR_UFUNC(power);
    REGISTER_SCALAR_UFUNC(add);
    REGISTER_SCALAR_UFUNC(subtract);

    /* quat, quat -> quat */

    arg_types[1] = hyperdual_descr->type_num;

    REGISTER_UFUNC(add);
    REGISTER_UFUNC(subtract);
    REGISTER_UFUNC(multiply);
    REGISTER_UFUNC(divide);
    REGISTER_UFUNC(power);
    REGISTER_UFUNC(copysign);

    PyModule_AddObject(m, "hyperdual", (PyObject *)&PyHyperdualArrType_Type);

    return m;
}
