#include "TpcGeometryOverride.h"

#include <g4detectors/PHG4TpcGeom.h>
#include <g4detectors/PHG4TpcGeomContainer.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <iostream>

TpcGeometryOverride::TpcGeometryOverride(const std::string &name)
  : SubsysReco(name)
{
}

int TpcGeometryOverride::InitRun(PHCompositeNode *topNode)
{
  auto *geomContainer = findNode::getClass<PHG4TpcGeomContainer>(topNode, "TPCGEOMCONTAINER");
  if (!geomContainer)
  {
    std::cout << Name() << "::InitRun - TPCGEOMCONTAINER not found" << std::endl;
    return Fun4AllReturnCodes::ABORTRUN;
  }

  PHG4TpcGeom *layer20 = geomContainer->GetLayerCellGeom(20);
  if (layer20)
  {
    std::cout << Name() << "::InitRun - input TPC geometry: place = ("
              << layer20->get_place_x() << ", " << layer20->get_place_y() << ", " << layer20->get_place_z()
              << ") cm, rot = (" << layer20->get_rot_x() << ", " << layer20->get_rot_y() << ", " << layer20->get_rot_z()
              << ") rad" << std::endl;
  }

  const auto range = geomContainer->get_begin_end();
  for (auto iter = range.first; iter != range.second; ++iter)
  {
    PHG4TpcGeom *geom = iter->second;
    geom->set_place_x(m_place[0]);
    geom->set_place_y(m_place[1]);
    geom->set_place_z(m_place[2]);
    geom->set_rot_x(m_rot[0]);
    geom->set_rot_y(m_rot[1]);
    geom->set_rot_z(m_rot[2]);
  }

  std::cout << Name() << "::InitRun - overridden TPC geometry: place = ("
            << m_place[0] << ", " << m_place[1] << ", " << m_place[2]
            << ") cm, rot = (" << m_rot[0] << ", " << m_rot[1] << ", " << m_rot[2]
            << ") rad" << std::endl;

  return Fun4AllReturnCodes::EVENT_OK;
}
