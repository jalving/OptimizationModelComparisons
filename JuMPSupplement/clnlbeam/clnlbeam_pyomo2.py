import clnlbeam_pyomo
from pyomo.environ import SolverFactory

solver = SolverFactory("ipopt")
solver.solve(clnlbeam_pyomo.model, tee=True)
