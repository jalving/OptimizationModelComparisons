import sys
import poek as pk
import poek.util

import nqueens_pyomo


model = pk.model()
pkmodel = pk.util.pyomo_to_poek(nqueens_pyomo.model)

opt = pk.solver('gurobi')
opt.set_option('TimeLimit', 0)
opt.solve(pkmodel)
