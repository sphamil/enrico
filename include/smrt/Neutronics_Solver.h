#ifndef Neutronics_Solver_h
#define Neutronics_Solver_h

#include <vector>

namespace enrico {

//===========================================================================//
/*!
 * \class Neutronics_Solver
 * \brief Base class for neutronics solvers.
 */
//===========================================================================//

class Neutronics_Solver {
public:
  //@{
  //! Public type aliases
  using Vec_Dbl = std::vector<double>;
  //@}

  // Constructor
  Neutronics_Solver() {}

  // Virtual destructor
  virtual ~Neutronics_Solver() {}

  // Solve for power given fuel temperature and coolant density
  virtual void solve(const Vec_Dbl& fuel_temperature,
                     const Vec_Dbl& coolant_density,
                     Vec_Dbl& power) = 0;

  // Update the temperature used in the neutronics solver
  virtual void update_temperature(const std::vector<double>& temperatures) {}

  // Update the density used in the neutronics solver
  virtual void update_density(const std::vector<double>& densities) {}
};

//---------------------------------------------------------------------------//
} // end namespace enrico

//---------------------------------------------------------------------------//
#endif // Neutronics_Solver_h

//---------------------------------------------------------------------------//
// end of Neutronics_Solver.h
//---------------------------------------------------------------------------//
