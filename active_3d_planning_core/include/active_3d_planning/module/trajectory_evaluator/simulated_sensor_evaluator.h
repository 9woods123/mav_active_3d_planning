#ifndef ACTIVE_3D_PLANNING_CORE_TRAJECTORY_EVALUATOR_SIMULATED_SENSOR_EVALUATOR_H
#define ACTIVE_3D_PLANNING_CORE_TRAJECTORY_EVALUATOR_SIMULATED_SENSOR_EVALUATOR_H

#include "active_3d_planning/module/sensor_model/sensor_model.h"
#include "active_3d_planning/module/trajectory_evaluator.h"
#include "active_3d_planning/map/occupancy_map.h"

#include <vector>

namespace active_3d_planning {
namespace evaluator_updater {
class SimulatedSensorUpdater;
}

namespace trajectory_evaluator {

// Base class for a group of evaluators that simulate a sensor in the future to
// get all visible voxels, then uses these to compute the gain. Can make use of
// the simulated sensor updater to update the gain of views without further
// raycasting.
class SimulatedSensorEvaluator : public TrajectoryEvaluator {
public:
  SimulatedSensorEvaluator(PlannerI &planner);
  // Override virtual functions
  bool computeGain(TrajectorySegment *traj_in) override;

  virtual void
  visualizeTrajectoryValue(VisualizationMarkers *markers,
                           const TrajectorySegment &trajectory) override;

  virtual void setupFromParamMap(Module::ParamMap *param_map) override;

protected:
  friend evaluator_updater::SimulatedSensorUpdater;
  // members
  std::unique_ptr<SensorModel> sensor_model_;
    map::OccupancyMap *map_;

  // parameters
  bool p_clear_from_parents_;
  bool p_visualize_sensor_view_;

  // store the visible voxel information, asign correct TrajectoryData type here
  virtual bool
  storeTrajectoryInformation(TrajectorySegment *traj_in,
                             const std::vector<Eigen::Vector3d> &new_voxels);

  // compute the gain from Trajectory information
  virtual bool computeGainFromVisibleVoxels(TrajectorySegment *traj_in) = 0;
};

// Information struct that is assigned to segments
struct SimulatedSensorInfo : public TrajectoryInfo {
  virtual ~SimulatedSensorInfo() {}

  std::vector<Eigen::Vector3d> visible_voxels;
};

} // namespace trajectory_evaluator
} // namespace active_3d_planning
#endif // MAV_ACTIVE_3D_PLANNING_TRAJECTORY_EVALUATORS_SIMULATED_SENSOR_EVALUATOR_H
