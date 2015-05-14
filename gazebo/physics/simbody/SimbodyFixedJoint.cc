/*
 * Copyright (C) 2012-2015 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include "gazebo/common/Assert.hh"
#include "gazebo/common/Console.hh"
#include "gazebo/common/Exception.hh"

#include "gazebo/physics/Model.hh"
#include "gazebo/physics/simbody/SimbodyLink.hh"
#include "gazebo/physics/simbody/SimbodyPhysics.hh"
#include "gazebo/physics/simbody/SimbodyFixedJoint.hh"

using namespace gazebo;
using namespace physics;

//////////////////////////////////////////////////
SimbodyFixedJoint::SimbodyFixedJoint(SimTK::MultibodySystem * /*_world*/,
                                     BasePtr _parent)
    : FixedJoint<SimbodyJoint>(_parent)
{
  this->physicsInitialized = false;
}

//////////////////////////////////////////////////
SimbodyFixedJoint::~SimbodyFixedJoint()
{
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::Load(sdf::ElementPtr _sdf)
{
  FixedJoint<SimbodyJoint>::Load(_sdf);
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::SetVelocity(unsigned int /*_index*/, double /*_angle*/)
{
  gzdbg << "SimbodyFixedJoint::SetVelocity: doesn't make sense in fixed joints...\n";
}

//////////////////////////////////////////////////
double SimbodyFixedJoint::GetVelocity(unsigned int /*index*/) const
{
  gzdbg << "SimbodyFixedJoint::GetVelocity: doesn't make sense in fixed joints...\n";
  return 0;
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::SetMaxForce(unsigned int /*_index*/, double /*_force*/)
{
  gzdbg << "SimbodyFixedJoint::SetMaxForce: doesn't make sense in simbody...\n";
}

//////////////////////////////////////////////////
double SimbodyFixedJoint::GetMaxForce(unsigned int /*index*/)
{
  gzdbg << "SimbodyFixedJoint::GetMaxForce: doesn't make sense in simbody...\n";
  return 0;
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::SetForceImpl(unsigned int /*_index*/, double /*_torque*/)
{
    gzdbg << "SimbodyFixedJoint::SetForceImpl: doesn't make sense in fixed joints...\n";
}

//////////////////////////////////////////////////
math::Vector3 SimbodyFixedJoint::GetGlobalAxis(unsigned int /*index*/) const
{
  gzdbg << "SimbodyFixedJoint::GetGlobalAxis: doesn't make sense in fixed joints...\n";
  return math::Vector3();
}

//////////////////////////////////////////////////
math::Angle SimbodyFixedJoint::GetAngleImpl(unsigned int /*_index*/) const
{
    gzdbg << "SimbodyFixedJoint::GetAngleImpl: doesn't make sense in fixed joints...\n";
    return math::Angle();
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::SaveSimbodyState(const SimTK::State &_state)
{
  if (!this->mobod.isEmptyHandle())
  {
    if (this->simbodyQ.empty())
      this->simbodyQ.resize(this->mobod.getNumQ(_state));

    if (this->simbodyU.empty())
      this->simbodyU.resize(this->mobod.getNumU(_state));

    for (unsigned int i = 0; i < this->simbodyQ.size(); ++i)
      this->simbodyQ[i] = this->mobod.getOneQ(_state, i);

    for (unsigned int i = 0; i < this->simbodyU.size(); ++i)
      this->simbodyU[i] = this->mobod.getOneU(_state, i);
  }
  else
  {
    // gzerr << "debug: joint name: " << this->GetScopedName() << "\n";
  }
}

//////////////////////////////////////////////////
void SimbodyFixedJoint::RestoreSimbodyState(SimTK::State &_state)
{
  if (!this->mobod.isEmptyHandle())
  {
    for (unsigned int i = 0; i < this->simbodyQ.size(); ++i)
      this->mobod.setOneQ(_state, i, this->simbodyQ[i]);

    for (unsigned int i = 0; i < this->simbodyU.size(); ++i)
      this->mobod.setOneU(_state, i, this->simbodyU[i]);
  }
  else
  {
    // gzerr << "restoring model [" << this->GetScopedName()
    //       << "] failed due to uninitialized mobod\n";
  }
}
