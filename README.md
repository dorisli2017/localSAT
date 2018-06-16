# GPLsat
Hypergraph partion based local search of SAT problem <br />

Pseodocode:<br />
1.Input: CNF Formula<br />
2.Graph construction (Hypergraph/ Dual Hypergraph/ None for small instances)<br />
3.Graph Partition (hMetis/ CFPM+FM/others)->see Literature (Hypergraph)<br />
4.Initial solution (Hard way(give cut a permenant satisfing initialization) <br />
============/soft way (control priority to lead up satisfy the cut first and recognize the status of cut variables)<br/>
                    )<br />
5.Parallel solver (Genetic combination using graph property(distence/independences)<br />
6.probalistische local search for one agent<br />
<br />
todo: 1 2 3 4 5<br /> 
doing: 6 (05.08-05.13) References (probSat & lazyWalker)<br />
