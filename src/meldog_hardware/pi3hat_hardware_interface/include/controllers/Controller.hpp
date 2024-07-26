#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "../pi3hat/pi3hat.h"
#include "bridges/ControllerBridge.hpp"
#include <memory>
#include <algorithm>

namespace controller_interface
{

/* Structure for basic actuator command */
struct ControllerCommand
{
    double position_;   /* [radians] */
    double velocity_;   /* [radians/s] */
    double torque_;     /* [Nm] */
};

/* Structure for basic actuator state */
struct ControllerState
{
    double position_;   /* [radians] */
    double velocity_;   /* [radians/s] */
    double torque_;     /* [Nm] */
    int temperature_;   /* [Celcius] */
    bool fault = false;
};

struct ControllerParameters
{
    double position_max_;
    double position_min_;
    double position_offset_;
    double velocity_max_;
    double torque_max_;
    int direction_ = 1;
    int id_;             /* Usage in your bridge (check moteus bridge)*/
    int bus_;            /* Usage in your bridge (check moteus bridge)*/
};

class Controller
{
    private:

    using CanFrame = mjbots::pi3hat::CanFrame;

    const std::unique_ptr<ControllerBridge> implementation_;
    ControllerParameters params_;


    public:
    Controller(std::unique_ptr<ControllerBridge> implementation, 
     const ControllerParameters& params);

    void make_command(CanFrame& tx_frame, ControllerCommand& command) const;
    void get_state(const CanFrame& rx_frame, ControllerState& state) const;
    void make_stop(CanFrame& tx_frame) const;

};

};
#endif