#ifndef GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H
#define GMX_TOOLS_DUMP_BUILDERS_TOPOLOGY_H

#include "gromacs/tools/dump/dump_builder.h"
#include "gromacs/tools/dump/dump_strategy.h"

#include "gromacs/topology/topology.h"
#include "gromacs/utility/strconvert.h"

class DumpBuilderTop : DumpBuilder {
private:
    const t_topology* top;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
public:
    DumpBuilderTop(const t_topology* top,
        gmx_bool bShowNumbers,
        gmx_bool bShowParameters) : top(top), bShowNumbers(bShowNumbers), bShowParameters(bShowParameters) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMTop : DumpBuilder {
private:
    const gmx_mtop_t* mtop;
    gmx_bool bShowNumbers;
    gmx_bool bShowParameters;
public:
    DumpBuilderMTop(const gmx_mtop_t* mtop,
        gmx_bool bShowNumbers,
        gmx_bool bShowParameters) : mtop(mtop), bShowNumbers(bShowNumbers), bShowParameters(bShowParameters) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderGroups : DumpBuilder {
private:
    const SimulationGroups groups;
    gmx_bool bShowNumbers;
public:
    DumpBuilderGroups(const SimulationGroups& groups, gmx_bool bShowNumbers) : groups(groups), bShowNumbers(bShowNumbers) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMolblock: DumpBuilder {
private:
    const std::vector<gmx_molblock_t>& molblock;
    const std::vector<gmx_moltype_t>& moltype;
public:
    DumpBuilderMolblock(const std::vector<gmx_molblock_t>& molblock, const std::vector<gmx_moltype_t>& moltype) : molblock(molblock), moltype(moltype) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderFFParams: DumpBuilder {
private:
    const gmx_ffparams_t& ffparams;
public:
    DumpBuilderFFParams(const gmx_ffparams_t& ffparams) : ffparams(ffparams) {}
    
    void build(DumpStrategy* strategy) override;
};

class DumpBuilderCmapGrid: DumpBuilder {
private:
    const gmx_cmap_t cmap;
public:
    DumpBuilderCmapGrid(const gmx_cmap_t& cmap) : cmap(cmap) {}

    void build(DumpStrategy* strategy) override;
};

class DumpBuilderMoltype: DumpBuilder {
private:
    const gmx_moltype_t* moltype;
    const int index;
    const gmx_ffparams_t& ffparams;
public:
    DumpBuilderMoltype(const gmx_moltype_t* moltype, int index, const gmx_ffparams_t& ffparams) : moltype(moltype), index(index), ffparams(ffparams) {}

    void build(DumpStrategy* strategy) override;
};

class DumpBuilderAtoms: DumpBuilder {
private:
    const t_atoms atoms;
public:
    DumpBuilderAtoms(const t_atoms& atoms) : atoms(atoms) {}

    void build(DumpStrategy* strategy) override;
};

class DumpBuilderListOfLists: DumpBuilder {
private:
    const std::string& title;
    const gmx::ListOfLists<int>& lists;
public:
    DumpBuilderListOfLists(const std::string& title, const gmx::ListOfLists<int>& lists) : title(title), lists(lists) {}

    void build(DumpStrategy* strategy) override;
};

class DumpBuilderInteractionList: DumpBuilder {
private:
    const std::string& title;
    const t_functype* functypes;
    const InteractionList& ilist;
    const t_iparams* iparams;
public:
    DumpBuilderInteractionList(const std::string& title, const t_functype* functype, const InteractionList& ilist, const t_iparams* iparams) :
        title(title), functypes(functype), ilist(ilist), iparams(iparams) {}

    void build(DumpStrategy* strategy) override;
};

#endif
