#include "dump_component_iparams.h"


std::vector<KeyFormatValue> getIParamsValues(t_functype ftype, const t_iparams& iparams)
{
    std::vector<KeyFormatValue> values;
    switch (ftype)
    {
        case F_ANGLES:
        case F_G96ANGLES:
            values.push_back({"rA", "%12.5e", iparams.harmonic.rA});
            values.push_back({"krA", "%12.5e", iparams.harmonic.krA});
            values.push_back({"rB", "%12.5e", iparams.harmonic.rB});
            values.push_back({"krB", "%12.5e", iparams.harmonic.krB});
            break;
        case F_CROSS_BOND_BONDS:
            values.push_back({"r1e", "%15.8e", iparams.cross_bb.r1e});
            values.push_back({"r2e", "%15.8e", iparams.cross_bb.r2e});
            values.push_back({"krr", "%15.8e", iparams.cross_bb.krr});
            break;
        case F_CROSS_BOND_ANGLES:
            values.push_back({"r1e", "%15.8e", iparams.cross_ba.r1e});
            values.push_back({"r2e", "%15.8e", iparams.cross_ba.r2e});
            values.push_back({"r3e", "%15.8e", iparams.cross_ba.r3e});
            values.push_back({"krt", "%15.8e", iparams.cross_ba.krt});
            break;
        case F_LINEAR_ANGLES:
            values.push_back({"klinA", "%15.8e", iparams.linangle.klinA});
            values.push_back({"aA", "%15.8e", iparams.linangle.aA});
            values.push_back({"klinB", "%15.8e", iparams.linangle.klinB});
            values.push_back({"aB", "%15.8e", iparams.linangle.aB});
            break;
        case F_UREY_BRADLEY:
            values.push_back({"thetaA", "%15.8e", iparams.u_b.thetaA});
            values.push_back({"kthetaA", "%15.8e", iparams.u_b.kthetaA});
            values.push_back({"r13A", "%15.8e", iparams.u_b.r13A});
            values.push_back({"kUBA", "%15.8e", iparams.u_b.kUBA});
            values.push_back({"thetaB", "%15.8e", iparams.u_b.thetaB});
            values.push_back({"kthetaB", "%15.8e", iparams.u_b.kthetaB});
            values.push_back({"r13B", "%15.8e", iparams.u_b.r13B});
            values.push_back({"kUBB", "%15.8e", iparams.u_b.kUBB});
            break;
        case F_QUARTIC_ANGLES:
            values.push_back({"theta", "%15.8e", iparams.qangle.theta});
            for (int i = 0; i < 5; i++)
            {
                const char c[3] = {'c', '0' + i, '\0'};
                values.push_back({c, "%15.8e", iparams.qangle.c[i]});
            }
            break;
        case F_BHAM:
            values.push_back({"a", "%15.8e", iparams.bham.a});
            values.push_back({"b", "%15.8e", iparams.bham.b});
            values.push_back({"c", "%15.8e", iparams.bham.c});
            break;
        case F_BONDS:
        case F_G96BONDS:
        case F_HARMONIC:
            values.push_back({"b0A", "%12.5e", iparams.harmonic.rA});
            values.push_back({"cbA", "%12.5e", iparams.harmonic.krA});
            values.push_back({"b0B", "%12.5e", iparams.harmonic.rB});
            values.push_back({"cbB", "%12.5e", iparams.harmonic.krB});
            break;
        case F_IDIHS:
            values.push_back({"xiA", "%12.5e", iparams.harmonic.rA});
            values.push_back({"cxA", "%12.5e", iparams.harmonic.krA});
            values.push_back({"xiB", "%12.5e", iparams.harmonic.rB});
            values.push_back({"cxB", "%12.5e", iparams.harmonic.krB});
            break;
        case F_MORSE:
            values.push_back({"b0A", "%15.8e", iparams.morse.b0A});
            values.push_back({"cbA", "%15.8e", iparams.morse.cbA});
            values.push_back({"betaA", "%15.8e", iparams.morse.betaA});
            values.push_back({"b0B", "%15.8e", iparams.morse.b0B});
            values.push_back({"cbB", "%15.8e", iparams.morse.cbB});
            values.push_back({"betaB", "%15.8e", iparams.morse.betaB});
            break;
        case F_CUBICBONDS:
            values.push_back({"b0", "%15.8e", iparams.cubic.b0});
            values.push_back({"kb", "%15.8e", iparams.cubic.kb});
            values.push_back({"kcub", "%15.8e", iparams.cubic.kcub});
            break;
        case F_CONNBONDS: break;
        case F_FENEBONDS:
            values.push_back({"bm", "%15.8e", iparams.fene.bm});
            values.push_back({"kb", "%15.8e", iparams.fene.kb});
            break;
        case F_RESTRBONDS:
            values.push_back({"lowA", "%15.8e", iparams.restraint.lowA});
            values.push_back({"up1A", "%15.8e", iparams.restraint.up1A});
            values.push_back({"up2A", "%15.8e", iparams.restraint.up2A});
            values.push_back({"kA", "%15.8e", iparams.restraint.kA});
            values.push_back({"lowB", "%15.8e", iparams.restraint.lowB});
            values.push_back({"up1B", "%15.8e", iparams.restraint.up1B});
            values.push_back({"up2B", "%15.8e", iparams.restraint.up2B});
            values.push_back({"kB", "%15.8e", iparams.restraint.kB});
            break;
        case F_TABBONDS:
        case F_TABBONDSNC:
        case F_TABANGLES:
        case F_TABDIHS:
            values.push_back({"tab", "%d", iparams.tab.table});
            values.push_back({"kA", "%15.8e", iparams.tab.kA});
            values.push_back({"kB", "%15.8e", iparams.tab.kB});
            break;
        case F_POLARIZATION:
            values.push_back({"alpha", "%15.8e", iparams.polarize.alpha});
            break;
        case F_ANHARM_POL:
            values.push_back({"alpha", "%15.8e", iparams.anharm_polarize.alpha});
            values.push_back({"drcut", "%15.8e", iparams.anharm_polarize.drcut});
            values.push_back({"khyp", "%15.8e", iparams.anharm_polarize.khyp});
            break;
        case F_THOLE_POL:
            values.push_back({"a", "%15.8e", iparams.thole.a});
            values.push_back({"alpha1", "%15.8e", iparams.thole.alpha1});
            values.push_back({"alpha2", "%15.8e", iparams.thole.alpha2});
            break;
        case F_WATER_POL:
            values.push_back({"al_x", "%15.8e", iparams.wpol.al_x});
            values.push_back({"al_y", "%15.8e", iparams.wpol.al_y});
            values.push_back({"al_z", "%15.8e", iparams.wpol.al_z});
            values.push_back({"rOH", "%9.6f", iparams.wpol.rOH});
            values.push_back({"rHH", "%9.6f", iparams.wpol.rHH});
            values.push_back({"rOD", "%9.6f", iparams.wpol.rOD});
            break;
        case F_LJ:
            values.push_back({"c6", "%15.8e", iparams.lj.c6});
            values.push_back({"c12", "%15.8e", iparams.lj.c12});
            break;
        case F_LJ14:
            values.push_back({"c6A", "%15.8e", iparams.lj14.c6A});
            values.push_back({"c12A", "%15.8e", iparams.lj14.c12A});
            values.push_back({"c6B", "%15.8e", iparams.lj14.c6B});
            values.push_back({"c12B", "%15.8e", iparams.lj14.c12B});
            break;
        case F_LJC14_Q:
            values.push_back({"fqq", "%15.8e", iparams.ljc14.fqq});
            values.push_back({"qi", "%15.8e", iparams.ljc14.qi});
            values.push_back({"qj", "%15.8e", iparams.ljc14.qj});
            values.push_back({"c6", "%15.8e", iparams.ljc14.c6});
            values.push_back({"c12", "%15.8e", iparams.ljc14.c12});
            break;
        case F_LJC_PAIRS_NB:
            values.push_back({"qi", "%15.8e", iparams.ljcnb.qi});
            values.push_back({"qj", "%15.8e", iparams.ljcnb.qj});
            values.push_back({"c6", "%15.8e", iparams.ljcnb.c6});
            values.push_back({"c12", "%15.8e", iparams.ljcnb.c12});
            break;
        case F_PDIHS:
        case F_PIDIHS:
        case F_ANGRES:
        case F_ANGRESZ:
            values.push_back({"phiA", "%15.8e", iparams.pdihs.phiA});
            values.push_back({"cpA", "%15.8e", iparams.pdihs.cpA});
            values.push_back({"phiB", "%15.8e", iparams.pdihs.phiB});
            values.push_back({"cpB", "%15.8e", iparams.pdihs.cpB});
            values.push_back({"mult", "%d", iparams.pdihs.mult});
            break;
        case F_DISRES:
            values.push_back({"label", "%4d", iparams.disres.label});
            values.push_back({"type", "%1d", iparams.disres.type});
            values.push_back({"low", "%15.8e", iparams.disres.low});
            values.push_back({"up1", "%15.8e", iparams.disres.up1});
            values.push_back({"up2", "%15.8e", iparams.disres.up2});
            values.push_back({"kfac", "%15.8e", iparams.disres.kfac});
            break;
        case F_ORIRES:
            values.push_back({"ex", "%4d", iparams.orires.ex});
            values.push_back({"label", "%4d", iparams.orires.label});
            values.push_back({"power", "%4d", iparams.orires.power});
            values.push_back({"c", "%15.8e", iparams.orires.c});
            values.push_back({"obs", "%15.8e", iparams.orires.obs});
            values.push_back({"kfac", "%15.8e", iparams.orires.kfac});
            break;
        case F_DIHRES:
            values.push_back({"phiA", "%15.8e", iparams.dihres.phiA});
            values.push_back({"dphiA", "%15.8e", iparams.dihres.dphiA});
            values.push_back({"kfacA", "%15.8e", iparams.dihres.kfacA});
            values.push_back({"phiB", "%15.8e", iparams.dihres.phiB});
            values.push_back({"dphiB", "%15.8e", iparams.dihres.dphiB});
            values.push_back({"kfacB", "%15.8e", iparams.dihres.kfacB});
            break;
        case F_POSRES:
            // TODO:
            // writer->writeLineFormatted(
            //         "pos0A=(%15.8e,%15.8e,%15.8e), fcA=(%15.8e,%15.8e,%15.8e), "
            //         "pos0B=(%15.8e,%15.8e,%15.8e), fcB=(%15.8e,%15.8e,%15.8e)",
            //         iparams.posres.pos0A[XX],
            //         iparams.posres.pos0A[YY],
            //         iparams.posres.pos0A[ZZ],
            //         iparams.posres.fcA[XX],
            //         iparams.posres.fcA[YY],
            //         iparams.posres.fcA[ZZ],
            //         iparams.posres.pos0B[XX],
            //         iparams.posres.pos0B[YY],
            //         iparams.posres.pos0B[ZZ],
            //         iparams.posres.fcB[XX],
            //         iparams.posres.fcB[YY],
            //         iparams.posres.fcB[ZZ]);
            break;
        case F_FBPOSRES:
            // TODO:
            // writer->writeLineFormatted(
            //         "pos0=(%15.8e,%15.8e,%15.8e), geometry=%d, r=%15.8e, k=%15.8e",
            //         iparams.fbposres.pos0[XX],
            //         iparams.fbposres.pos0[YY],
            //         iparams.fbposres.pos0[ZZ],
            //         iparams.fbposres.geom,
            //         iparams.fbposres.r,
            //         iparams.fbposres.k);
            break;
        case F_RBDIHS:
            // TODO:
            // for (int i = 0; i < NR_RBDIHS; i++)
            // {
            //     writer->writeStringFormatted(
            //             "%srbcA[%d]=%15.8e", i == 0 ? "" : ", ", i, iparams.rbdihs.rbcA[i]);
            // }
            // writer->ensureLineBreak();
            // for (int i = 0; i < NR_RBDIHS; i++)
            // {
            //     writer->writeStringFormatted(
            //             "%srbcB[%d]=%15.8e", i == 0 ? "" : ", ", i, iparams.rbdihs.rbcB[i]);
            // }
            // writer->ensureLineBreak();
            break;
        case F_FOURDIHS:
        {
            /* Use the OPLS -> Ryckaert-Bellemans formula backwards to get
             * the OPLS potential constants back.
             */
            // TODO:
            // const real* rbcA = iparams.rbdihs.rbcA;
            // const real* rbcB = iparams.rbdihs.rbcB;
            // real        VA[4], VB[4];

            // VA[3] = -0.25 * rbcA[4];
            // VA[2] = -0.5 * rbcA[3];
            // VA[1] = 4.0 * VA[3] - rbcA[2];
            // VA[0] = 3.0 * VA[2] - 2.0 * rbcA[1];

            // VB[3] = -0.25 * rbcB[4];
            // VB[2] = -0.5 * rbcB[3];
            // VB[1] = 4.0 * VB[3] - rbcB[2];
            // VB[0] = 3.0 * VB[2] - 2.0 * rbcB[1];

            // for (int i = 0; i < NR_FOURDIHS; i++)
            // {
            //     writer->writeStringFormatted("%sFourA[%d]=%15.8e", i == 0 ? "" : ", ", i, VA[i]);
            // }
            // writer->ensureLineBreak();
            // for (int i = 0; i < NR_FOURDIHS; i++)
            // {
            //     writer->writeStringFormatted("%sFourB[%d]=%15.8e", i == 0 ? "" : ", ", i, VB[i]);
            // }
            // writer->ensureLineBreak();
            break;
        }

        case F_CONSTR:
        case F_CONSTRNC:
            values.push_back({"dA", "%15.8e", iparams.constr.dA});
            values.push_back({"dB", "%15.8e", iparams.constr.dB});
            break;
        case F_SETTLE:
            values.push_back({"doh", "%15.8e", iparams.settle.doh});
            values.push_back({"dhh", "%15.8e", iparams.settle.dhh});
            break;
        case F_VSITE1: break;
        case F_VSITE2:
        case F_VSITE2FD:
            values.push_back({"a", "%15.8e", iparams.vsite.a}); break;
        case F_VSITE3:
        case F_VSITE3FD:
        case F_VSITE3FAD:
            values.push_back({"a", "%15.8e", iparams.vsite.a});
            values.push_back({"b", "%15.8e", iparams.vsite.b});
            break;
        case F_VSITE3OUT:
        case F_VSITE4FD:
        case F_VSITE4FDN:
            values.push_back({"a", "%15.8e", iparams.vsite.a});
            values.push_back({"b", "%15.8e", iparams.vsite.b});
            values.push_back({"c", "%15.8e", iparams.vsite.c});
            break;
        case F_VSITEN:
            values.push_back({"n", "%2d", iparams.vsiten.n});
            values.push_back({"a", "%15.8e", iparams.vsiten.a});
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
            values.push_back({"cmapA", "%1d", iparams.cmap.cmapA});
            values.push_back({"cmapB", "%1d", iparams.cmap.cmapB});
            break;
        case F_RESTRANGLES: 
            values.push_back({"costheta0A", "%12.5e", iparams.harmonic.rA});
            values.push_back({"kthetaA", "%12.5e", iparams.harmonic.krA});
            values.push_back({"costheta0B", "%12.5e", iparams.harmonic.rB});
            values.push_back({"kthetaB", "%12.5e", iparams.harmonic.krB});
        case F_RESTRDIHS:
            values.push_back({"phiA", "%12.5e", iparams.pdihs.phiA});
            values.push_back({"cpA", "%12.5e", iparams.pdihs.cpA});
            break;
        case F_CBTDIHS:
            // TODO:
            // writer->writeLineFormatted("kphi=%15.8e", iparams.cbtdihs.cbtcA[0]);
            // for (int i = 1; i < NR_CBTDIHS; i++)
            // {
            //     writer->writeStringFormatted(", cbtcA[%d]=%15.8e", i - 1, iparams.cbtdihs.cbtcA[i]);
            // }
            // writer->ensureLineBreak();
            break;
        default:
            gmx_fatal(FARGS,
                      "unknown function type %d (%s) in %s line %d",
                      ftype,
                      interaction_function[ftype].name,
                      __FILE__,
                      __LINE__);
    }
    return values;
}
