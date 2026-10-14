#ifndef TPC_LASERCLUSTERHELPER_H
#define TPC_LASERCLUSTERHELPER_H

#include <fun4all/SubsysReco.h>


#include <trackbase/ActsGeometry.h>
#include <trackbase/TrkrDefs.h>

#include <array>
#include <memory>
#include <string>

class ActsGeometry;
class LaserCluster;
class PHCompositeNode;
class PHG4TpcGeomContainer;
class PHGarfield;
class TPolyLine3D;

class LaserClusterHelper : public SubsysReco
{
  public:
    LaserClusterHelper ();
    ~LaserClusterHelper() override;
    LaserClusterHelper(const LaserClusterHelper&) = delete;
    LaserClusterHelper& operator=(const LaserClusterHelper&) = delete;
    LaserClusterHelper(LaserClusterHelper&&) = delete;
    LaserClusterHelper& operator=(LaserClusterHelper&&) = delete;

    void loadNodes(PHCompositeNode *topNode);
  
    Acts::Vector3 getHitPosition(TrkrDefs::hitsetkey, TrkrDefs::hitkey) const;
    Acts::Vector3 getClusterCentroid(LaserCluster*) const;
    std::array<double, 3> getClusterHardwareCentroid(LaserCluster*) const;
    Acts::Vector3 getClusterCentroidWithPHGarfield(LaserCluster*) const;

    void set_useZ(bool use) { m_useZ = use; }
    void set_useGlobal(bool use) { m_useGlobal = use; }
    void set_useDouble(bool use) { m_useDouble = use; }
    void set_useGarfield(bool use) { m_useGarfield = use; }    
    void set_garfield_cmvoltage(double use) { m_garfield_cmvoltage = use; }
    void set_garfield_zerofield(bool use) { m_garfield_zerofield = use; }
    void set_garfield_keffside0(double use) { m_garfield_keffside0 = use; m_manual_garfield_keffside0 = true; }
    void set_garfield_keffside1(double use) { m_garfield_keffside1 = use; m_manual_garfield_keffside1 = true; }
    void set_garfield_stepns(double use) { m_garfield_stepns = use; }

    //! 2D (axisymmetric) space-charge E-field map; empty = CDB Tpc_PolySeeding_EField
    void set_garfield_efieldmap(const std::string &file) { m_garfield_efieldmap = file; }
    //! side-separated 3D space-charge E-field maps; takes precedence over the 2D map
    void set_garfield_efieldmap3d(const std::string &side0, const std::string &side1)
    {
      m_garfield_efieldmap3d[0] = side0;
      m_garfield_efieldmap3d[1] = side1;
    }

    //! TPC placement used by PHGarfield for global <-> TPC-frame transforms
    enum class GarfieldTpcGeometry
    {
      HardcodedSurvey = 0,  //! fixed endcap-survey numbers (previous default)
      GeomContainer = 1,    //! place/rot from TPCGEOMCONTAINER, same transform as ActsGeometry
      Perfect = 2           //! no displacement, no tilt
    };
    void set_garfield_tpc_geometry(GarfieldTpcGeometry geom) { m_garfield_tpc_geometry = geom; }

  private:

    ActsGeometry *m_tGeometry{nullptr};
    PHG4TpcGeomContainer *m_geom_container{nullptr};
    mutable std::unique_ptr<PHGarfield> m_phgarfield;

    bool m_useZ{false};
    bool m_useGlobal{true};
    bool m_useDouble{false};
    bool m_useGarfield{false};

    double m_garfield_cmvoltage{380.0};
    bool m_garfield_zerofield{false};
    bool m_manual_garfield_keffside0{false};
    bool m_manual_garfield_keffside1{false};
    double m_garfield_keffside0{1.0};
    double m_garfield_keffside1{1.0};
    double m_garfield_stepns{50.0};
    std::string m_garfield_efieldmap;
    std::array<std::string, 2> m_garfield_efieldmap3d;
    GarfieldTpcGeometry m_garfield_tpc_geometry{GarfieldTpcGeometry::HardcodedSurvey};


};

#endif
