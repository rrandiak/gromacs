#include "gromacs/tools/dump/builders/inputrec.h"

#include <cstdio>

#include <gtest/gtest.h>

#include "gromacs/mdtypes/md_enums.h"
#include "gromacs/tools/dump/builders/grp_opts.h"
#include "gromacs/tools/dump/builders/qm_opts.h"
#include "gromacs/utility/real.h"

#include "test_base_builder.h"
#include "test_base_pr_func.h"


namespace gmx
{

namespace test
{

static void populateInputRecord(t_inputrec& ir)
{
    {
        ir.useMts = true;
        MtsLevel level;
        level.forceGroups.set(static_cast<int>(MtsForceGroups::Pull));
        level.stepFactor = 0;
        ir.mtsLevels.push_back(level);
        level.forceGroups.set(static_cast<int>(MtsForceGroups::Pull));
        level.stepFactor = 2;
        ir.mtsLevels.push_back(level);
    }

    ir.ensembleTemperatureSetting = EnsembleTemperatureSetting::NotAvailable;

    ir.pressureCouplingOptions.epc = PressureCoupling::Mttk;

    {
        ir.bPull = true;
        ir.pull  = std::unique_ptr<pull_params_t>(new pull_params_t());
    }

    ir.params = new KeyValueTreeObject();

    {
        t_grpopts opts;
        opts.ngtc   = 2;
        opts.ngacc  = 1;
        opts.ngfrz  = 1;
        opts.ngener = 1;
        opts.nrdf   = new real[opts.ngtc]{ 1.0, 2.0 };
        opts.ref_t  = new real[opts.ngtc]{ 300.0, 400.0 };
        opts.annealing = new SimulatedAnnealing[2]{ SimulatedAnnealing::No, SimulatedAnnealing::Periodic };
        opts.anneal_npoints = new int[opts.ngtc]{ 2, 2 };
        opts.anneal_time    = new real*[opts.ngtc];
        opts.anneal_temp    = new real*[opts.ngtc];
        for (int i = 0; i < opts.ngtc; ++i)
        {
            opts.anneal_time[i] = new real[2]{ 0.0, 1.0 };
            opts.anneal_temp[i] = new real[2]{ 295.0, 305.0 };
        }
        opts.tau_t        = new real[opts.ngtc]{ 10.0, 20.0 };
        opts.acceleration = new real[opts.ngacc][3]{ { 0.2, 0.3, 0.5 } };
        opts.nFreeze      = new int[opts.ngfrz][3]{ { 1, 2, 3 } };
        opts.egp_flags    = new int[opts.ngener]{ 1 };
        ir.opts           = opts;
    }
}


TEST_F(PrintFunctionTest, GeneratesCorrect_pr_inputrec_Repr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    auto func = std::bind(pr_inputrec, std::placeholders::_1, 0, "inputrec", &ir, false);

    runTest(func, { "data/inputrec.txt", "data/qm_opts.txt", "data/grp_opts.txt" });
}

TEST_F(TextRepresentationTest, GeneratesCorrectInputRecordRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    InputRecBuilder builder(&ir);

    runTest(&builder, "data/inputrec.txt");
}

TEST_F(JsonRepresentationTest, GeneratesCorrectInputRecordRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    InputRecBuilder builder(&ir);

    runTest(&builder, "data/inputrec.json");
}

TEST_F(YamlRepresentationTest, GeneratesCorrectInputRecordRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    InputRecBuilder builder(&ir);

    runTest(&builder, "data/inputrec.yaml");
}

TEST_F(TextRepresentationTest, GeneratesCorrectQmOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    QmOptionsBuilder builder(&ir);

    runTest(&builder, "data/qm_opts.txt");
}

TEST_F(JsonRepresentationTest, GeneratesCorrectQmOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    QmOptionsBuilder builder(&ir);

    runTest(&builder, "data/qm_opts.json");
}

TEST_F(YamlRepresentationTest, GeneratesCorrectQmOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    QmOptionsBuilder builder(&ir);

    runTest(&builder, "data/qm_opts.yaml");
}

TEST_F(TextRepresentationTest, GeneratesCorrectGroupOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    GroupOptionsBuilder builder(&ir.opts);

    runTest(&builder, "data/grp_opts.txt");
}

TEST_F(JsonRepresentationTest, GeneratesCorrectGroupOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    GroupOptionsBuilder builder(&ir.opts);

    runTest(&builder, "data/grp_opts.json");
}

TEST_F(YamlRepresentationTest, GeneratesCorrectGroupOptionsRepr)
{
    t_inputrec ir;
    populateInputRecord(ir);

    GroupOptionsBuilder builder(&ir.opts);

    runTest(&builder, "data/grp_opts.yaml");
}

} // namespace test

} // namespace gmx
