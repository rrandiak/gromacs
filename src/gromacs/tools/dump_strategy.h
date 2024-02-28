#ifndef GMX_TOOLS_DUMP_STRATEGY_H
#define GMX_TOOLS_DUMP_STRATEGY_H

#include "gromacs/utility/real.h"
#include "gromacs/utility/iserializer.h"
#include "gromacs/utility/basedefinitions.h"

#include <cstdio>

#include "gromacs/tools/dump_component.h"

#define DIM 3

using Value = std::variant<std::string, int, int64_t, long unsigned int, double, real, gmx_bool>;
using vec = Value[DIM];

class DumpStrategy {
public:
    virtual ~DumpStrategy() = default;

    //! Returns whether \p is available (not null), and prints a note if it is not.
    virtual bool available(const void* p, const char* title) = 0;

    //! Prints a filename.
    virtual void pr_filename(const char* filename) = 0;

    //! Prints a title for a dumped section.
    virtual void pr_title(const char* title) = 0;

    //! Prints a title for a dumped section with a index suffixed.
    virtual void pr_title_i(const char* title, int i) = 0;

    //! Prints a title for a dumped section with a dimension suffixed.
    virtual void pr_title_n(const char* title, int n) = 0;

    //! Prints a title for a dumped section with 2D dimension suffixed (in NxM format).
    virtual void pr_title_nxn(const char* title, int n1, int n2) = 0;

    virtual void close_section() = 0;

    virtual void pr_is_present(const char* title, gmx_bool b) = 0;

    //! Prints a named value.
    virtual void pr_named_value(const char* name, const Value& value) = 0;

    virtual void pr_matrix(const char* title, const rvec* m, gmx_bool bMDPformat) = 0;

    virtual void pr_rvec(const char* title, const rvec vec, int n, gmx_bool bShowNumbers) = 0;

    virtual void pr_rvecs(const char* title, const rvec vec[], int n) = 0;

    virtual void pr_ivec(const char* title, const int vec[], int n, gmx_bool bShowNumbers) = 0;
    
    virtual void pr_ivecs(const char* title, const ivec vec[], int n) = 0;

    virtual void pr_ivec_block(const char* title, const int vec[], int n, gmx_bool bShowNumbers) = 0;

    // //! Prints a string value.
    // virtual void pr_str(const char* title, const char* s) = 0;

    // //! Prints an integer value.
    // virtual void pr_int(const char* title, int i) = 0;

    // //! Prints a int64_t value.
    // virtual void pr_int64(const char* title, int64_t i) = 0;

    // //! Prints a double-precision floating-point value.
    // virtual void pr_double(const char* title, double d) = 0;

    // //! Prints a floating-point value.
    // virtual void pr_real(const char* title, real r) = 0;

    // //! Prints strings as a section; intended to be used for an array of names.
    // virtual void pr_strings(const char* title, const char* const* const* nm, int n, gmx_bool bShowNumbers) = 0;

    // //! Prints an array of doubles.
    // virtual void pr_doubles(const char* title, const double* vec, int n) = 0;

    // //! Prints an array of reals.
    // virtual void pr_reals(const char* title, const real vec[], int n) = 0;

    // //! Prints an array of reals as a matrix with inner dimension dim.
    // virtual void pr_reals_of_dim(const char* title, const real* vec, int n, int dim) = 0;
};

#endif
