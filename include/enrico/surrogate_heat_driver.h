//! \file heat_driver.h
//! Driver for Magnolia's heat transfer solver
#ifndef ENRICO_SURROGATE_HEAT_DRIVER_H
#define ENRICO_SURROGATE_HEAT_DRIVER_H

#include "enrico/heat_fluids_driver.h"
#include "gsl/gsl"
#include "mpi.h"
#include "pugixml.hpp"
#include "xtensor/xtensor.hpp"

#include <cstddef>

namespace enrico {

class SurrogateHeatDriver : public HeatFluidsDriver {
public:
  //! Initializes heat-fluids surrogate with the given MPI communicator.
  //!
  //! \param comm  The MPI communicator used to initialze the surrogate
  //! \param node  XML node containing settings for surrogate
  explicit SurrogateHeatDriver(MPI_Comm comm, double pressure_bc, pugi::xml_node node);

  bool has_coupling_data() const final { return true; }

  //! Solves the heat-fluids surrogate solver
  void solve_step() final;

  //! Returns Number of rings in fuel and clad
  std::size_t n_rings() { return n_fuel_rings_ + n_clad_rings_; }

  //! Write data to VTK
  void write_step(int timestep, int iteration) final;

  xt::xtensor<double, 1> temperature() const final;

  xt::xtensor<double, 1> density() const final;

  xt::xtensor<int, 1> fluid_mask() const final;

  //! Returns temperature in [K] for given region
  double temperature(int pin, int axial, int ring) const;

  // Data on fuel pins
  xt::xtensor<double, 2> pin_centers_; //!< (x,y) values for center of fuel pins
  xt::xtensor<double, 1> z_;           //!< Bounding z-values for axial segments
  std::size_t n_pins_;                 //!< number of fuel pins
  std::size_t n_axial_;                //!< number of axial segments

  // Dimensions for a single fuel pin axial segment
  double clad_outer_radius_;     //!< clad outer radius in [cm]
  double clad_inner_radius_;     //!< clad inner radius in [cm]
  double pellet_radius_;         //!< fuel pellet radius in [cm]
  std::size_t n_fuel_rings_{20}; //!< number of fuel rings
  std::size_t n_clad_rings_{2};  //!< number of clad rings

  // solver variables and settings
  double tol_;                         //!< tolerance on convergence
  xt::xtensor<double, 3> source_;      //!< heat source for each (axial segment, ring)
  xt::xtensor<double, 1> r_grid_clad_; //!< radii of each clad ring in [cm]
  xt::xtensor<double, 1> r_grid_fuel_; //!< radii of each fuel ring in [cm]

  // visualization
  std::string viz_basename_{
    "heat_surrogate"}; //!< base filename for visualization files (default: magnolia)
  std::string viz_iterations_{
    "none"};                    //!< visualization iterations to write (none, all, final)
  std::string viz_data_{"all"}; //!< visualization data to write
  std::string viz_regions_{"all"}; //!< visualization regions to write
  size_t vtk_radial_res_{20};      //!< radial resolution of resulting vtk files

private:
  //! Create internal arrays used for heat equation solver
  void generate_arrays();

  //!< temperature in [K] for each (pin, axial segment, ring)
  xt::xtensor<double, 3> temperature_;

  //!< density in [g/cm^3] for each (pin, axial segment, ring)
  xt::xtensor<double, 3> density_;

  //! Value is 1 if (pin, axial segment, ring) region is in fluid; otherwise 0
  //!
  //! Because the surrogate only solves heat and only represents solid, this whole xtensor
  //! == 0
  xt::xtensor<int, 3> fluid_mask_;

}; // end SurrogateHeatDriver

} // namespace enrico

#endif // ENRICO_SURROGATE_HEAT_DRIVER_H
