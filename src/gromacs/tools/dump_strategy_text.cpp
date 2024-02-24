#include "gromacs/tools/dump_strategy_text.h"

bool DumpStrategyText::available(const void* p, const char* title) {
    if (!p) {
        componentsStack.top()->addTextLeaf(title, "not available");
    }
    return (p != nullptr);
}

void DumpStrategyText::pr_filename(const char* filename) {
    componentsStack.top()->addTextSection(filename);
}

void DumpStrategyText::pr_title(const char* title) {
    TextDumpComponent* comp = componentsStack.top()->addTextSection(title);
    componentsStack.push(comp);
}

void DumpStrategyText::pr_title_i(const char* title, int i) {
    // fprintf(fp, "%s (%d):\n", title, n);
}

void DumpStrategyText::pr_title_n(const char* title, int n) {
    // fprintf(fp, "%s (%d):\n", title, n);
}

void DumpStrategyText::pr_title_nxn(const char* title, int n1, int n2) {
    int bufferSize = snprintf(nullptr, 0, "%s (%dx%d)", title, n1, n2) + 1;
    std::string formattedString(bufferSize, '\0');
    snprintf(&formattedString[0], bufferSize, "%s (%dx%d)", title, n1, n2);

    TextDumpComponent* comp = componentsStack.top()->addTextObject(formattedString.c_str());
    componentsStack.push(comp);
}

void DumpStrategyText::pr_named_value(const char* name, const Value& value) {
    componentsStack.top()->addTextLeaf(name, value);
}

void DumpStrategyText::pr_matrix(const char* title, const rvec* m, gmx_bool bMDPformat) {
    if (bMDPformat) {
        // fprintf(fp,
        //         "%-10s    = %g %g %g %g %g %g\n",
        //         title,
        //         m[XX][XX],
        //         m[YY][YY],
        //         m[ZZ][ZZ],
        //         m[XX][YY],
        //         m[XX][ZZ],
        //         m[YY][ZZ]);
    } else {
        pr_rvecs(title, m, DIM);
    }
}

void DumpStrategyText::pr_rvec(const char* title, const rvec vec, int n, gmx_bool bShowNumbers) {
}

void DumpStrategyText::pr_rvecs(const char* title, const rvec vec[], int n)
{
    const char* fshort = "%12.5e";
    const char* flong  = "%15.8e";
    const char* format = (getenv("GMX_PRINT_LONGFORMAT") != nullptr) ? flong : fshort;
    int         i, j;

    if (available(vec, title)) {
        pr_title_nxn(title, n, DIM);
        TextDumpComponent* comp = componentsStack.top();

        for (i = 0; i < n; i++) {
            comp->addFormattedTextLeaf("%s[%5d]=", title, i);
            comp->addTextVectorLeaf(vec[i], DIM);
            // fprintf(fp, "%s[%5d]={", title, i);
            // for (j = 0; j < DIM; j++)
            // {
            //     if (j != 0)
            //     {
            //         fprintf(fp, ", ");
            //     }
            //     fprintf(fp, format, vec[i][j]);
            // }
            // fprintf(fp, "}\n");
        }

        componentsStack.pop();
    }
}

void DumpStrategyText::pr_ivec(const char* title, const int vec[], int n, gmx_bool bShowNumbers)
{
}
    
void DumpStrategyText::pr_ivecs(const char* title, const ivec vec[], int n)
{
}

void DumpStrategyText::pr_ivec_block(const char* title, const int vec[], int n, gmx_bool bShowNumbers)
{
}

// void DumpStrategyText::pr_reals(const char* title, const real* vec, int n)
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

// void DumpStrategyText::pr_doubles(const char* title, const double* vec, int n)
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

// void DumpStrategyText::pr_reals_of_dim(const char* title, const real* vec, int n, int dim)
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

// void DumpStrategyText::pr_int(const char* title, int i)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %d\n", title, i);
// }

// void DumpStrategyText::pr_int64(const char* title, int64_t i)
// {
//     // char buf[STEPSTRSIZE];

//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, gmx_step_str(i, buf));
// }

// void DumpStrategyText::pr_real(const char* title, real r)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, r);
// }

// void DumpStrategyText::pr_double(const char* title, double d)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %g\n", title, d);
// }

// void DumpStrategyText::pr_str(const char* title, const char* s)
// {
//     // pr_indent(fp, indent);
//     // fprintf(fp, "%-30s = %s\n", title, s);
//     componentsStack.top()->addFormattedTextLeaf("%-30s = %s\n", title, s);
// }

// void DumpStrategyText::pr_strings(const char* title, const char* const* const* nm, int n, gmx_bool bShowNumbers)
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
