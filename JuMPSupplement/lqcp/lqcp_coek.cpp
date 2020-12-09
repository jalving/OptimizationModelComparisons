#include <cmath>
#include <cstdlib>
#include <vector>
#include "coek/coek.hpp"


double yt(int j, double dx) {
  return 0.5*(1 - (j*dx)*(j*dx));
}


int main(int argc, char** argv) {
  
  int n = atoi(argv[1]);
  int m = n;
  int n1 = n-1;
  double dx = 1.0/n;
  double T = 1.58;
  double dt = T/m;
  double h2 = dx*dx;
  double a = 0.001;

  coek::Model model;

  std::vector<std::vector<coek::Variable> > y(m+1, std::vector<coek::Variable>(n+1));
  for (int i = 0; i <= m; i++)
    for (int j = 0; j <= n; j++)
      y[i][j] = model.add_variable(0, 1, 0);
  
  std::vector<coek::Variable> u(m+1);
  for (int i = 1; i <= m; i++) 
    u[i] = model.add_variable(-1, 1, 0);


  // OBJECTIVE  
  // First term
  coek::Expression term1;
    term1 +=   ( y[m][0] - yt(0,dx) ) * ( y[m][0] - yt(0,dx) );
  for (int j = 1; j <= n-1; j++) {
    term1 += 2*( y[m][j] - yt(j,dx) ) * ( y[m][j] - yt(j,dx) );
  }
    term1 +=   ( y[m][n] - yt(n,dx) ) * ( y[m][n] - yt(n,dx) );

  // Second term
  coek::Expression term2;
  for (int i = 1; i <= m-1; i++)
    term2 += 2*u[i]*u[i];
  term2 += u[m]*u[m];

  model.add_objective(0.25*dx*term1 + 0.25*a*dt*term2);


  // PDE
  for (int i = 0; i < m; i++) {
    for (int j = 1; j < n; j++) {
      model.add_constraint( y[i+1][j] - y[i][j] == dt*0.5/h2*(y[i][j-1] - 2*y[i][j] + y[i][j+1] + y[i+1][j-1] - 2*y[i+1][j] + y[i+1][j+1]) );
    }
  }


  // IC
  for (int j = 0; j <= n; j++) {
    model.add_constraint( y[0][j] == 0 );
  }


  // BC
  for (int i = 1; i <= m; i++) {
    model.add_constraint( y[i][2] - 4*y[i][1] + 3*y[i][0] == 0 );
    model.add_constraint( (y[i][n-2] - 4*y[i][n1] + 3*y[i][n])/(2*dx) == u[i]-y[i][n]);
  }
  
  
  coek::Solver opt;
  opt.initialize("gurobi");
  opt.set_option("TimeLimit", 0);
  opt.solve(model);

  return 0;
}
