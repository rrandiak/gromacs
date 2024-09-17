#include <gtest/gtest.h>
#include <cstdio>

#include "gromacs/tools/dump/builders/grp_opts.h"
#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/utility/real.h"

#include "builder_test.h"

namespace gmx
{

namespace test
{

static t_grpopts createGroupOptions()
{
    t_grpopts opts;
    opts.ngtc = 2;
    opts.ngacc = 1;
    opts.ngfrz = 1;
    opts.ngener = 1;
    opts.nrdf = new real[opts.ngtc]{1.0, 2.0};
    opts.ref_t = new real[opts.ngtc]{300.0, 400.0};
    opts.annealing = new SimulatedAnnealing[2]{SimulatedAnnealing::No, SimulatedAnnealing::Periodic};
    opts.anneal_npoints = new int[opts.ngtc]{2, 2};
    opts.anneal_time = new real*[opts.ngtc];
    opts.anneal_temp = new real*[opts.ngtc];
    for (int i = 0; i < opts.ngtc; ++i)
    {
        opts.anneal_time[i] = new real[2]{0.0, 1.0};
        opts.anneal_temp[i] = new real[2]{295.0, 305.0};
    }
    opts.tau_t = new real[opts.ngtc]{10.0, 20.0};
    opts.acceleration = new real[opts.ngacc][3]{
        {0.2, 0.3, 0.5}
    };
    opts.nFreeze = new int[opts.ngfrz][3]{
        {1, 2, 3}
    };
    opts.egp_flags = new int[opts.ngener]{1};

    return opts;
}

static void destroyGroupOptions(t_grpopts& opts)
{
    delete[] opts.nrdf;
    delete[] opts.ref_t;
    delete[] opts.annealing;
    delete[] opts.anneal_npoints;
    for (int i = 0; i < opts.ngtc; ++i)
    {
        delete[] opts.anneal_time[i];
        delete[] opts.anneal_temp[i];
    }
    delete[] opts.anneal_time;
    delete[] opts.anneal_temp;
    delete[] opts.tau_t;
    delete[] opts.acceleration;
    delete[] opts.nFreeze;
    delete[] opts.egp_flags;
}

TEST_F(DumpBuilderTextTest, GeneratesCorrectGroupOptionsDumpText)
{
    t_grpopts opts = createGroupOptions();
    GroupOptionsBuilder builder = GroupOptionsBuilder(&opts);

    // Verify the output
    runTest(&builder,
        "\n"
        "grpopts:\n"
        "   nrdf:           1           2\n"
        "   ref-t:         300         400\n"
        "   tau-t:          10          20\n"
        "annealing:          No    Periodic\n"
        "annealing-npoints:           2           2\n"
        "annealing-time [0]:\t         0.0         1.0\n"
        "annealing-temp [0]:\t       295.0       305.0\n"
        "annealing-time [1]:\t         0.0         1.0\n"
        "annealing-temp [1]:\t       295.0       305.0\n"
        "   acc:\t         0.2         0.3         0.5\n"
        "   nfreeze:           Y           Y           Y\n"
        "   energygrp-flags[  0]: 1"
    );

    destroyGroupOptions(opts);
}

TEST_F(DumpBuilderYamlTest, GeneratesCorrectGroupOptionsDumpYaml)
{
    t_grpopts opts = createGroupOptions();
    GroupOptionsBuilder builder = GroupOptionsBuilder(&opts);

    // Verify the output
    runTest(&builder,
        "---\n"
		"grpopts:\n"
		"  nrdf: [1, 2]\n"
		"  ref-t: [300, 400]\n"
		"  tau-t: [10, 20]\n"
		"annealing: [No, Periodic]\n"
		"annealing-npoints: [2, 2]\n"
		"annealing-time: [0, 1]\n"
		"annealing-temp: [295, 305]\n"
		"annealing-time: [0, 1]\n"
		"annealing-temp: [295, 305]\n"
		"acc: [0.2, 0.3, 0.5]\n"
		"nfreeze: [Y, Y, Y]\n"
		"energygrp-flags:\n"
		"  - [1]\n"
    );

    destroyGroupOptions(opts);
}

TEST_F(DumpBuilderJsonTest, GeneratesCorrectGroupOptionsDumpJsonPretty)
{
    t_grpopts opts = createGroupOptions();
    GroupOptionsBuilder builder = GroupOptionsBuilder(&opts);

    // Verify the output
    runTest(&builder,
        "{\n"
		"  \"grpopts\": {\n"
		"    \"nrdf\": [\n"
		"      1,\n"
		"      2\n"
		"    ],\n"
		"    \"ref-t\": [\n"
		"      300,\n"
		"      400\n"
		"    ],\n"
		"    \"tau-t\": [\n"
		"      10,\n"
		"      20\n"
		"    ]\n"
		"  },\n"
		"  \"annealing\": [\n"
		"    \"No\",\n"
		"    \"Periodic\"\n"
		"  ],\n"
		"  \"annealing-npoints\": [\n"
		"    2,\n"
		"    2\n"
		"  ],\n"
		"  \"annealing-time\": [\n"
		"    0,\n"
		"    1\n"
		"  ],\n"
		"  \"annealing-temp\": [\n"
		"    295,\n"
		"    305\n"
		"  ],\n"
		"  \"annealing-time\": [\n"
		"    0,\n"
		"    1\n"
		"  ],\n"
		"  \"annealing-temp\": [\n"
		"    295,\n"
		"    305\n"
		"  ],\n"
		"  \"acc\": [\n"
		"    0.2,\n"
		"    0.3,\n"
		"    0.5\n"
		"  ],\n"
		"  \"nfreeze\": [\n"
		"    \"Y\",\n"
		"    \"Y\",\n"
		"    \"Y\"\n"
		"  ],\n"
		"  \"energygrp-flags\": [\n"
		"    [\n"
		"      1\n"
		"    ]\n"
		"  ]\n"
		"}\n"
    );

    destroyGroupOptions(opts);
}

} // namespace test

} // namespace gmx
