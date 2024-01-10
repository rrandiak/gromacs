#include "gromacs/tools/dump_json_strategy.h"

bool DumpJsonStrategy::available(const void* p, const char* title) {
    if (!p) {
        componentsStack.top()->addJsonLeaf(title, "not_available");
    }
    return (p != nullptr);
}

void DumpJsonStrategy::pr_filename(const char* filename) {
    componentsStack.top()->addJsonLeaf("filename", filename);
}

void DumpJsonStrategy::pr_title(const char* title) {
    JsonDumpComponent* comp = componentsStack.top()->addJsonObject(title);
    componentsStack.push(comp);
}

void DumpJsonStrategy::pr_title_n(const char* title, int n) {
    // fprintf(fp, "%s (%d):\n", title, n);
}

void DumpJsonStrategy::pr_title_nxn(const char* title, int n1, int n2) {
    // fprintf(fp, "%s (%dx%d):\n", title, n1, n2);

}

void DumpJsonStrategy::pr_named_value(const char* name, const Value& value) {
    componentsStack.top()->addJsonLeaf(name, value);
}

void DumpJsonStrategy::pr_matrix(const char* title, const rvec* m, gmx_bool bMDPformat) {
}

void DumpJsonStrategy::pr_rvecs(const char* title, const rvec vec[], int n) {
    const char* fshort = "%12.5e";
    const char* flong  = "%15.8e";
    const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title)) {
        pr_title_nxn(title, n, DIM);

        for (i = 0; i < n; i++) {
        }
    }
    // const char* fshort = "%12.5e";
    // const char* flong  = "%15.8e";
    // const char* format;
    // int         i, j;

    // if (getenv("GMX_PRINT_LONGFORMAT") != nullptr)
    // {
    //     format = flong;
    // }
    // else
    // {
    //     format = fshort;
    // }

    // if (available(fp, vec, indent, title))
    // {
    //     indent = pr_title_nxn(fp, indent, title, n, DIM);
    //     for (i = 0; i < n; i++)
    //     {
    //         pr_indent(fp, indent);
    //         fprintf(fp, "%s[%5d]={", title, i);
    //         for (j = 0; j < DIM; j++)
    //         {
    //             if (j != 0)
    //             {
    //                 fprintf(fp, ", ");
    //             }
    //             fprintf(fp, format, vec[i][j]);
    //         }
    //         fprintf(fp, "}\n");
    //     }
    // }
}


// void DumpJsonStrategy::pr_reals(const char* title, const real* vec, int n)
// {
//     // if (available(fp, vec, indent, title))
//     // {
//     //     pr_indent(fp, indent);
//     //     fprintf(fp, "%s:\t", title);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         fprintf(fp, "  %10g", vec[i]);
//     //     }
//     //     fprintf(fp, "\n");
//     // }
// }

// void DumpJsonStrategy::pr_doubles(const char* title, const double* vec, int n)
// {
//     // if (available(fp, vec, indent, title))
//     // {
//     //     pr_indent(fp, indent);
//     //     fprintf(fp, "%s:\t", title);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         fprintf(fp, "  %10g", vec[i]);
//     //     }
//     //     fprintf(fp, "\n");
//     // }
// }

// void DumpJsonStrategy::pr_reals_of_dim(const char* title, const real* vec, int n, int dim)
// {
//     // const char* fshort = "%12.5e";
//     // const char* flong  = "%15.8e";
//     // const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;

//     // if (available(fp, vec, indent, title))
//     // {
//     //     indent = pr_title_nxn(fp, indent, title, n, dim);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         pr_indent(fp, indent);
//     //         fprintf(fp, "%s[%5d]={", title, i);
//     //         for (int j = 0; j < dim; j++)
//     //         {
//     //             if (j != 0)
//     //             {
//     //                 fprintf(fp, ", ");
//     //             }
//     //             fprintf(fp, format, vec[i * dim + j]);
//     //         }
//     //         fprintf(fp, "}\n");
//     //     }
//     // }
// }

// void DumpJsonStrategy::pr_int(const char* title, int i)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %d\n", title, i);
// }

// void DumpJsonStrategy::pr_int64(const char* title, int64_t i)
// {
//     // char buf[STEPSTRSIZE];

//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, gmx_step_str(i, buf));
// }

// void DumpJsonStrategy::pr_real(const char* title, real r)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, r);
// }

// void DumpJsonStrategy::pr_double(const char* title, double d)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, d);
// }

// void DumpJsonStrategy::pr_str(const char* title, const char* s)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, s);
//     componentsStack.top()->addFormattedTextLeaf("%-30s = %s\n", title, s);
// }

// void DumpJsonStrategy::pr_strings(const char* title, const char* const* const* nm, int n, gmx_bool bShowNumbers)
// {
//     // if (available(fp, nm, indent, title))
//     // {
//     //     indent = pr_title_n(fp, indent, title, n);
//     //     for (int i = 0; i < n; i++)
//     //     {
//     //         pr_indent(fp, indent);
//     //         fprintf(fp, "%s[%d]={name=\"%s\"}\n", title, bShowNumbers ? i : -1, *(nm[i]));
//     //     }
//     // }
// }

