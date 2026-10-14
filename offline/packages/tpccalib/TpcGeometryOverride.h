#ifndef TPCCALIB_TPCGEOMETRYOVERRIDE_H
#define TPCCALIB_TPCGEOMETRYOVERRIDE_H

#include <fun4all/SubsysReco.h>

#include <string>

class PHCompositeNode;

/*
 * Overwrites the TPC envelope placement (place_x/y/z, cm) and tilt
 * (rot_x/y/z, rad) stored in every TPCGEOMCONTAINER layer, e.g. to run
 * reconstruction with a perfect TPC instead of the survey position that the
 * Tracking_Geometry CDB file was produced with.
 *
 * Must be registered BEFORE MakeActsGeometry (i.e. before TrackingInit()),
 * since MakeActsGeometry::InitRun builds the TPC envelope<->world transform
 * from these values.
 */
class TpcGeometryOverride : public SubsysReco
{
 public:
  explicit TpcGeometryOverride(const std::string &name = "TpcGeometryOverride");
  ~TpcGeometryOverride() override = default;

  int InitRun(PHCompositeNode *topNode) override;

  void set_place(double x, double y, double z)
  {
    m_place[0] = x;
    m_place[1] = y;
    m_place[2] = z;
  }
  void set_rot(double x, double y, double z)
  {
    m_rot[0] = x;
    m_rot[1] = y;
    m_rot[2] = z;
  }

  //! zero displacement and tilt
  void set_perfect()
  {
    set_place(0, 0, 0);
    set_rot(0, 0, 0);
  }

  //! survey position, same numbers as G4TPC::tpc_survey_position in G4_TrkrSimulation.C
  void set_survey()
  {
    set_place(-1.6775 / 10.0, -0.336 / 10.0, -7.1365 / 10.0);
    set_rot(-0.00029817, 0.0014833, 0.0);
  }

 private:
  double m_place[3]{0, 0, 0};
  double m_rot[3]{0, 0, 0};
};

#endif
