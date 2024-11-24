#include "gromacs/tools/reprs/components/iparams_component.h"

#include "gromacs/utility/fatalerror.h"

void printInteractionParameters(t_functype ftype, const t_iparams& iparams, ReprFormatter* strategy)
{
    switch (ftype)
    {
        case F_ANGLES:
        case F_G96ANGLES:
            strategy->pr_iparam("thA", "%12.5e", iparams.harmonic.rA);
            strategy->pr_iparam("ctA", "%12.5e", iparams.harmonic.krA);
            strategy->pr_iparam("thB", "%12.5e", iparams.harmonic.rB);
            strategy->pr_iparam("ctB", "%12.5e", iparams.harmonic.krB);
            break;
        case F_CROSS_BOND_BONDS:
            strategy->pr_iparam("r1e", "%15.8e", iparams.cross_bb.r1e);
            strategy->pr_iparam("r2e", "%15.8e", iparams.cross_bb.r2e);
            strategy->pr_iparam("krr", "%15.8e", iparams.cross_bb.krr);
            break;
            // Dump Inconsistency: 2nd 'r1e' instead of 'r2e'
        case F_CROSS_BOND_ANGLES:
            strategy->pr_iparam("r1e", "%15.8e", iparams.cross_ba.r1e);
            strategy->pr_iparam("r2e", "%15.8e", iparams.cross_ba.r2e);
            strategy->pr_iparam("r3e", "%15.8e", iparams.cross_ba.r3e);
            strategy->pr_iparam("krt", "%15.8e", iparams.cross_ba.krt);
            break;
        case F_LINEAR_ANGLES:
            strategy->pr_iparam("klinA", "%15.8e", iparams.linangle.klinA);
            strategy->pr_iparam("aA", "%15.8e", iparams.linangle.aA);
            strategy->pr_iparam("klinB", "%15.8e", iparams.linangle.klinB);
            strategy->pr_iparam("aB", "%15.8e", iparams.linangle.aB);
            break;
        case F_UREY_BRADLEY:
            strategy->pr_iparam("thetaA", "%15.8e", iparams.u_b.thetaA);
            strategy->pr_iparam("kthetaA", "%15.8e", iparams.u_b.kthetaA);
            strategy->pr_iparam("r13A", "%15.8e", iparams.u_b.r13A);
            strategy->pr_iparam("kUBA", "%15.8e", iparams.u_b.kUBA);
            strategy->pr_iparam("thetaB", "%15.8e", iparams.u_b.thetaB);
            strategy->pr_iparam("kthetaB", "%15.8e", iparams.u_b.kthetaB);
            strategy->pr_iparam("r13B", "%15.8e", iparams.u_b.r13B);
            strategy->pr_iparam("kUBB", "%15.8e", iparams.u_b.kUBB);
            break;
        case F_QUARTIC_ANGLES:
            strategy->pr_iparam("theta", "%15.8e", iparams.qangle.theta);
            for (int i = 0; i < 5; i++)
            {
                strategy->pr_iparam(gmx::formatString("c%d", i), "%15.8e", iparams.qangle.c[i]);
            }
            break;
        case F_BHAM:
            strategy->pr_iparam("a", "%15.8e", iparams.bham.a);
            strategy->pr_iparam("b", "%15.8e", iparams.bham.b);
            strategy->pr_iparam("c", "%15.8e", iparams.bham.c);
            break;
        case F_BONDS:
        case F_G96BONDS:
        case F_HARMONIC:
            strategy->pr_iparam("b0A", "%12.5e", iparams.harmonic.rA);
            strategy->pr_iparam("cbA", "%12.5e", iparams.harmonic.krA);
            strategy->pr_iparam("b0B", "%12.5e", iparams.harmonic.rB);
            strategy->pr_iparam("cbB", "%12.5e", iparams.harmonic.krB);
            break;
        case F_IDIHS:
            strategy->pr_iparam("xiA", "%12.5e", iparams.harmonic.rA);
            strategy->pr_iparam("cxA", "%12.5e", iparams.harmonic.krA);
            strategy->pr_iparam("xiB", "%12.5e", iparams.harmonic.rB);
            strategy->pr_iparam("cxB", "%12.5e", iparams.harmonic.krB);
            break;
        case F_MORSE:
            strategy->pr_iparam("b0A", "%15.8e", iparams.morse.b0A);
            strategy->pr_iparam("cbA", "%15.8e", iparams.morse.cbA);
            strategy->pr_iparam("betaA", "%15.8e", iparams.morse.betaA);
            strategy->pr_iparam("b0B", "%15.8e", iparams.morse.b0B);
            strategy->pr_iparam("cbB", "%15.8e", iparams.morse.cbB);
            strategy->pr_iparam("betaB", "%15.8e", iparams.morse.betaB);
            break;
        case F_CUBICBONDS:
            strategy->pr_iparam("b0", "%15.8e", iparams.cubic.b0);
            strategy->pr_iparam("kb", "%15.8e", iparams.cubic.kb);
            strategy->pr_iparam("kcub", "%15.8e", iparams.cubic.kcub);
            break;
            // Dump Inconsistency - in old implementation 'writer->ensureEmptyLine()' does not function as expected
        case F_CONNBONDS: break;
        case F_FENEBONDS:
            strategy->pr_iparam("bm", "%15.8e", iparams.fene.bm);
            strategy->pr_iparam("kb", "%15.8e", iparams.fene.kb);
            break;
            // Dump Inconsistency: ',' comma after kB value
        case F_RESTRBONDS:
            strategy->pr_iparam("lowA", "%15.8e", iparams.restraint.lowA);
            strategy->pr_iparam("up1A", "%15.8e", iparams.restraint.up1A);
            strategy->pr_iparam("up2A", "%15.8e", iparams.restraint.up2A);
            strategy->pr_iparam("kA", "%15.8e", iparams.restraint.kA);
            strategy->pr_iparam("lowB", "%15.8e", iparams.restraint.lowB);
            strategy->pr_iparam("up1B", "%15.8e", iparams.restraint.up1B);
            strategy->pr_iparam("up2B", "%15.8e", iparams.restraint.up2B);
            strategy->pr_iparam("kB", "%15.8e,", iparams.restraint.kB);
            break;
        case F_TABBONDS:
        case F_TABBONDSNC:
        case F_TABANGLES:
        case F_TABDIHS:
            strategy->pr_iparam("tab", "%d", iparams.tab.table);
            strategy->pr_iparam("kA", "%15.8e", iparams.tab.kA);
            strategy->pr_iparam("kB", "%15.8e", iparams.tab.kB);
            break;
        case F_POLARIZATION: strategy->pr_iparam("alpha", "%15.8e", iparams.polarize.alpha); break;
        case F_ANHARM_POL:
            strategy->pr_iparam("alpha", "%15.8e", iparams.anharm_polarize.alpha);
            strategy->pr_iparam("drcut", "%15.8e", iparams.anharm_polarize.drcut);
            strategy->pr_iparam("khyp", "%15.8e", iparams.anharm_polarize.khyp);
            break;
        case F_THOLE_POL:
            strategy->pr_iparam("a", "%15.8e", iparams.thole.a);
            strategy->pr_iparam("alpha1", "%15.8e", iparams.thole.alpha1);
            strategy->pr_iparam("alpha2", "%15.8e", iparams.thole.alpha2);
            break;
        case F_WATER_POL:
            strategy->pr_iparam("al_x", "%15.8e", iparams.wpol.al_x);
            strategy->pr_iparam("al_y", "%15.8e", iparams.wpol.al_y);
            strategy->pr_iparam("al_z", "%15.8e", iparams.wpol.al_z);
            strategy->pr_iparam("rOH", "%9.6f", iparams.wpol.rOH);
            strategy->pr_iparam("rHH", "%9.6f", iparams.wpol.rHH);
            strategy->pr_iparam("rOD", "%9.6f", iparams.wpol.rOD);
            break;
        case F_LJ:
            strategy->pr_iparam("c6", "%15.8e", iparams.lj.c6);
            strategy->pr_iparam("c12", "%15.8e", iparams.lj.c12);
            break;
        case F_LJ14:
            strategy->pr_iparam("c6A", "%15.8e", iparams.lj14.c6A);
            strategy->pr_iparam("c12A", "%15.8e", iparams.lj14.c12A);
            strategy->pr_iparam("c6B", "%15.8e", iparams.lj14.c6B);
            strategy->pr_iparam("c12B", "%15.8e", iparams.lj14.c12B);
            break;
        case F_LJC14_Q:
            strategy->pr_iparam("fqq", "%15.8e", iparams.ljc14.fqq);
            strategy->pr_iparam("qi", "%15.8e", iparams.ljc14.qi);
            strategy->pr_iparam("qj", "%15.8e", iparams.ljc14.qj);
            strategy->pr_iparam("c6", "%15.8e", iparams.ljc14.c6);
            strategy->pr_iparam("c12", "%15.8e", iparams.ljc14.c12);
            break;
        case F_LJC_PAIRS_NB:
            strategy->pr_iparam("qi", "%15.8e", iparams.ljcnb.qi);
            strategy->pr_iparam("qj", "%15.8e", iparams.ljcnb.qj);
            strategy->pr_iparam("c6", "%15.8e", iparams.ljcnb.c6);
            strategy->pr_iparam("c12", "%15.8e", iparams.ljcnb.c12);
            break;
        case F_PDIHS:
        case F_PIDIHS:
        case F_ANGRES:
        case F_ANGRESZ:
            strategy->pr_iparam("phiA", "%15.8e", iparams.pdihs.phiA);
            strategy->pr_iparam("cpA", "%15.8e", iparams.pdihs.cpA);
            strategy->pr_iparam("phiB", "%15.8e", iparams.pdihs.phiB);
            strategy->pr_iparam("cpB", "%15.8e", iparams.pdihs.cpB);
            strategy->pr_iparam("mult", "%d", iparams.pdihs.mult);
            break;
            // Dump Inconsistency: ')' at the end
        case F_DISRES:
            strategy->pr_iparam("label", "%4d", iparams.disres.label);
            strategy->pr_iparam("type", "%1d", iparams.disres.type);
            strategy->pr_iparam("low", "%15.8e", iparams.disres.low);
            strategy->pr_iparam("up1", "%15.8e", iparams.disres.up1);
            strategy->pr_iparam("up2", "%15.8e", iparams.disres.up2);
            strategy->pr_iparam("fac", "%15.8e)", iparams.disres.kfac);
            break;
            // Dump Inconsistency: ')' at the end
            // Dump Inconsistency: label diffrently aligned then with F_DISRES
        case F_ORIRES:
            strategy->pr_iparam("ex", "%4d", iparams.orires.ex);
            strategy->pr_iparam("label", "%d", iparams.orires.label);
            strategy->pr_iparam("power", "%4d", iparams.orires.power);
            strategy->pr_iparam("c", "%15.8e", iparams.orires.c);
            strategy->pr_iparam("obs", "%15.8e", iparams.orires.obs);
            strategy->pr_iparam("kfac", "%15.8e)", iparams.orires.kfac);
            break;
        case F_DIHRES:
            strategy->pr_iparam("phiA", "%15.8e", iparams.dihres.phiA);
            strategy->pr_iparam("dphiA", "%15.8e", iparams.dihres.dphiA);
            strategy->pr_iparam("kfacA", "%15.8e", iparams.dihres.kfacA);
            strategy->pr_iparam("phiB", "%15.8e", iparams.dihres.phiB);
            strategy->pr_iparam("dphiB", "%15.8e", iparams.dihres.dphiB);
            strategy->pr_iparam("kfacB", "%15.8e", iparams.dihres.kfacB);
            break;
        case F_POSRES:
        {
            real pos0A[DIM] = { iparams.posres.pos0A[XX], iparams.posres.pos0A[YY], iparams.posres.pos0A[ZZ] };
            strategy->pr_iparam_reals_of_dim("pos0A", "%15.8e", pos0A);
            real fcA[DIM] = { iparams.posres.fcA[XX], iparams.posres.fcA[YY], iparams.posres.fcA[ZZ] };
            strategy->pr_iparam_reals_of_dim("fcA", "%15.8e", fcA);
            real pos0B[DIM] = { iparams.posres.pos0B[XX], iparams.posres.pos0B[YY], iparams.posres.pos0B[ZZ] };
            strategy->pr_iparam_reals_of_dim("pos0B", "%15.8e", pos0B);
            real fcB[DIM] = { iparams.posres.fcB[XX], iparams.posres.fcB[YY], iparams.posres.fcB[ZZ] };
            strategy->pr_iparam_reals_of_dim("fcB", "%15.8e", fcB);
            break;
        }
        case F_FBPOSRES:
        {
            real pos0[DIM] = { iparams.posres.fcB[XX], iparams.posres.fcB[YY], iparams.posres.fcB[ZZ] };
            strategy->pr_iparam_reals_of_dim("pos0", "%15.8e", pos0);
            strategy->pr_iparam("geom", "%d", iparams.fbposres.geom);
            strategy->pr_iparam("r", "%15.8e", iparams.fbposres.r);
            strategy->pr_iparam("k", "%15.8e", iparams.fbposres.k);
            break;
        }
        case F_RBDIHS:
            for (int i = 0; i < NR_RBDIHS; i++)
            {
                strategy->pr_iparam(gmx::formatString("rbcA[%d]", i), "%15.8e", iparams.rbdihs.rbcA[i]);
            }
            strategy->pr_iparam_ensure_line_break();
            for (int i = 0; i < NR_RBDIHS; i++)
            {
                strategy->pr_iparam(gmx::formatString("rbcB[%d]", i), "%15.8e", iparams.rbdihs.rbcB[i]);
            }
            break;
        case F_FOURDIHS:
        {
            /* Use the OPLS -> Ryckaert-Bellemans formula backwards to get
             * the OPLS potential constants back.
             */
            const real* rbcA = iparams.rbdihs.rbcA;
            const real* rbcB = iparams.rbdihs.rbcB;
            real        VA[4], VB[4];

            VA[3] = -0.25 * rbcA[4];
            VA[2] = -0.5 * rbcA[3];
            VA[1] = 4.0 * VA[3] - rbcA[2];
            VA[0] = 3.0 * VA[2] - 2.0 * rbcA[1];

            VB[3] = -0.25 * rbcB[4];
            VB[2] = -0.5 * rbcB[3];
            VB[1] = 4.0 * VB[3] - rbcB[2];
            VB[0] = 3.0 * VB[2] - 2.0 * rbcB[1];

            for (int i = 0; i < NR_FOURDIHS; i++)
            {
                strategy->pr_iparam(gmx::formatString("FourA[%d]", i), "%15.8e", VA[i]);
            }
            strategy->pr_iparam_ensure_line_break();
            for (int i = 0; i < NR_FOURDIHS; i++)
            {
                strategy->pr_iparam(gmx::formatString("FourB[%d]", i), "%15.8e", VB[i]);
            }
            break;
        }
        case F_CONSTR:
        case F_CONSTRNC:
            strategy->pr_iparam("dA", "%15.8e", iparams.constr.dA);
            strategy->pr_iparam("dB", "%15.8e", iparams.constr.dB);
            break;
        case F_SETTLE:
            strategy->pr_iparam("doh", "%15.8e", iparams.settle.doh);
            strategy->pr_iparam("dhh", "%15.8e", iparams.settle.dhh);
            break;
        case F_VSITE1: break;
        case F_VSITE2:
        case F_VSITE2FD: strategy->pr_iparam("a", "%15.8e", iparams.vsite.a); break;
        case F_VSITE3:
        case F_VSITE3FD:
        case F_VSITE3FAD:
            strategy->pr_iparam("a", "%15.8e", iparams.vsite.a);
            strategy->pr_iparam("b", "%15.8e", iparams.vsite.b);
            break;
        case F_VSITE3OUT:
        case F_VSITE4FD:
        case F_VSITE4FDN:
            strategy->pr_iparam("a", "%15.8e", iparams.vsite.a);
            strategy->pr_iparam("b", "%15.8e", iparams.vsite.b);
            strategy->pr_iparam("c", "%15.8e", iparams.vsite.c);
            break;
        case F_VSITEN:
            strategy->pr_iparam("n", "%2d", iparams.vsiten.n);
            strategy->pr_iparam("a", "%15.8e", iparams.vsiten.a);
            break;
        case F_GB12_NOLONGERUSED:
        case F_GB13_NOLONGERUSED:
        case F_GB14_NOLONGERUSED:
            // These could only be generated by grompp, not written in
            // a .top file. Now that implicit solvent is not
            // supported, they can't be generated, and the values are
            // ignored if read from an old .tpr file. So there is
            // nothing to print.
            break;
        case F_CMAP:
            strategy->pr_iparam("cmapA", "%1d", iparams.cmap.cmapA);
            strategy->pr_iparam("cmapB", "%1d", iparams.cmap.cmapB);
            break;
        case F_RESTRANGLES:
            strategy->pr_iparam("costheta0A", "%12.5e", iparams.harmonic.rA);
            strategy->pr_iparam("kthetaA", "%12.5e", iparams.harmonic.krA);
            strategy->pr_iparam("costheta0B", "%12.5e", iparams.harmonic.rB);
            strategy->pr_iparam("kthetaB", "%12.5e", iparams.harmonic.krB);
            break;
        case F_RESTRDIHS:
            strategy->pr_iparam("phiA", "%12.5e", iparams.pdihs.phiA);
            strategy->pr_iparam("cpA", "%12.5e", iparams.pdihs.cpA);
            break;
            // Dump Inconsistency: both new line and comma between parameters
        case F_CBTDIHS:
            strategy->pr_iparam("kphi", "%15.8e", iparams.cbtdihs.cbtcA[0]);
            strategy->pr_iparam_ensure_line_break();
            for (int i = 1; i < NR_CBTDIHS; i++)
            {
                strategy->pr_iparam(
                        gmx::formatString("cbtcA[%d]", i - 1), "%15.8e", iparams.cbtdihs.cbtcA[i]);
            }
            break;
        default:
            gmx_fatal(FARGS,
                      "unknown function type %d (%s) in %s line %d",
                      ftype,
                      interaction_function[ftype].name,
                      __FILE__,
                      __LINE__);
    }
}
