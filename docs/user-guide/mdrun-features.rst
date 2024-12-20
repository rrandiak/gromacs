Useful mdrun features
=======================
This section discusses features in :ref:`gmx mdrun` that do not fit well
elsewhere.

.. _single-point energy:

Re-running a simulation
-----------------------
The rerun feature allows you to take any trajectory file ``traj.trr``
and compute quantities based upon the coordinates in that file using
the model physics supplied in the ``topol.tpr`` file. It can be used
with command lines like ``mdrun -s topol -rerun traj.trr``. That :ref:`tpr`
could be different from the one that generated the trajectory. This
can be used to compute the energy or forces for exactly the
coordinates supplied as input, or to extract quantities based on
subsets of the molecular system (see :ref:`gmx convert-tpr` and
:ref:`gmx trjconv`). It is easier to do a correct "single-point" energy
evaluation with this feature than a 0-step simulation.

Neighbor searching is performed for every frame in the trajectory
independently of the value in :mdp:`nstlist`, since
:ref:`gmx mdrun` can no longer assume anything about how the
structures were generated. Naturally, no update or constraint
algorithms are ever used.

The rerun feature cannot, in general, compute many of the quantities
reported during full simulations. It does only take positions as input
(ignoring potentially present velocities), and does only report potential
energies, volume and density, dH/dl terms, and restraint information.
It does notably not report kinetic, total or conserved energy, temperature,
virial or pressure.

Running a simulation in reproducible mode
-----------------------------------------
It is generally difficult to run an efficient parallel MD simulation
that is based primarily on floating-point arithmetic and is fully
reproducible. By default, :ref:`gmx mdrun` will observe how things are going
and vary how the simulation is conducted in order to optimize
throughput. However, there is a "reproducible mode" available with
``mdrun -reprod`` that will systematically eliminate all sources of
variation within that run; repeated invocations on the same input and
hardware will be binary identical. However, running in this mode on
different hardware, or with a different compiler, etc. will not be
reproducible. This should normally only be used when investigating
possible problems.

Halting running simulations
---------------------------

When :ref:`gmx mdrun` receives a TERM or INT signal (e.g. when ctrl+C is
pressed), it will stop at the next neighbor search step or at the
second global communication step, whichever happens later.
When :ref:`gmx mdrun` receives a second TERM or INT signal and
reproducibility is not requested, it will stop at the first global
communication step.
In both cases all the usual output will be written to file and
a checkpoint file is written at the last step.
When :ref:`gmx mdrun` receives an ABRT signal or the third TERM or INT signal,
it will abort directly without writing a new checkpoint file.
When running with MPI, a signal to one of the :ref:`gmx mdrun` ranks
is sufficient, this signal should not be sent to mpirun or
the :ref:`gmx mdrun` process that is the parent of the others.

Running multi-simulations
-------------------------
There are numerous situations where running a related set of
simulations within the same invocation of mdrun are necessary or
useful. Running a replica-exchange simulation requires it, as do
simulations using ensemble-based distance or orientation restraints.
Running a related series of lambda points for a free-energy
computation is also convenient to do this way, but beware of the potential
side-effects related to resource utilization and load balance discussed later.

This feature requires
configuring |Gromacs| with an :ref:`external MPI library <mpi-support>`
so that the set of
simulations can communicate. The ``n`` simulations within the set can
use internal MPI parallelism also, so that ``mpirun -np x gmx_mpi mdrun``
for ``x`` a multiple of ``n`` will use ``x/n`` ranks per simulation.

To launch a multi-simulation, the ``-multidir`` option is used.
For the input and output files of a multi-simulation a set of ``n`` subdirectories is required,
one for each simulation.
Place all the relevant input files in those directories (e.g. named
``topol.tpr``), and launch a multi-simualtion with
``mpirun -np x gmx_mpi mdrun -s topol -multidir <names-of-directories>``.
If the order of the simulations
within the multi-simulation is significant, you are responsible
for ordering their names when you provide them to ``-multidir``. Be
careful with shells that do filename globbing dictionary-style, e.g.
``dir1 dir10 dir11 ... dir2 ...``.

Examples running multi-simulations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

::

    mpirun -np 32 gmx_mpi mdrun -multidir a b c d

Starts a multi-simulation on 32 ranks with 4 simulations. The input
and output files are found in directories ``a``, ``b``, ``c``, and ``d``.

::

    mpirun -np 32 gmx_mpi mdrun -multidir a b c d -gputasks 0000000011111111

Starts the same multi-simulation as before. On a machine with two
physical nodes and two GPUs per node, there will be 16 MPI ranks per
node, and 8 MPI ranks per simulation. The 16 MPI ranks doing PP work
on a node are mapped to the GPUs with IDs 0 and 1, even though they
come from more than one simulation. They are mapped in the order
indicated, so that the PP ranks from each simulation use a single
GPU. However, the order ``0101010101010101`` could run faster.

Running replica-exchange simulations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

When running a multi-simulation, using ``gmx mdrun -replex n`` means that a
replica exchange is attempted every given number of steps. The number
of replicas is set with ``-multidir`` option, described
above.  All run input files should use a different value for the
coupling parameter (e.g. temperature), which ascends over the set of
input files. The random seed for replica exchange is set with
``-reseed``. After every exchange, the velocities are scaled and
neighbor searching is performed. See the Reference Manual for more
details on how replica exchange functions in |Gromacs|.

Multi-simulation performance considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The frequency of communication across a multi-simulation can have an impact
on performance. This is highly algorithm dependent, but in general it is
recommended to set up a multi-simulation to do inter-simulation communication
as infrequently as possible but as frequently as necessary.
However, even when members of multi-simulation do not communicate frequently (or at all),
and therefore the associated performance overhead is small or even negligible,
load imbalance can still have a significant impact on performance and resource utilization.
Current multi-simulation algorithms use a fixed interval for data exchange (e.g.
replica exchange every ``N`` steps) and therefore all members of a multi-simulation
need to reach this step before the collective communication can happen and
any of them can proceed to step ``N+1``. Hence, the slowest member of the
multi-simulation will determine the performance of the entire ensemble.
This load imbalance will not only limit performance but will also leave resources
idle; e.g. if one of the simulations in an ``n``-way multi-simulation runs at half
the performance of the rest, the resources assigned to the ``n-1``
faster running simulations will be left idle for approximately half of the
wall-time of the entire multi-simulation job.
The source of this imbalance can range from inherent workload imbalance across
the simulations within a multi-simulation to differences in hardware speed or
inter-node network performance variability affecting a subset of ranks and therefore
only some of the simulations.
Reducing the amount of resources left idle requires reducing the load imbalance,
which may involve splitting up non-communicating multi-simulations, or making sure
to request a "compact" allocation on a cluster (if the job scheduler allows).
Note that imbalance also applies to non-communicating multi-simulations like FEP
calculations since the resources assigned to earlier finishing simulations can not
be relinquished until the entire MPI job can finish.

Controlling the length of the simulation
----------------------------------------

Normally, the length of an MD simulation is best managed through the
:ref:`mdp` option :mdp:`nsteps`, however there are situations where
more control is useful. :samp:`gmx mdrun -nsteps 100` overrides the :ref:`mdp`
file and executes 100 steps. :samp:`gmx mdrun -maxh 2.5` will terminate the
simulation shortly before 2.5 hours elapse, which can be useful when
running under cluster queues (as long as the queuing system does not
ever suspend the simulation).

