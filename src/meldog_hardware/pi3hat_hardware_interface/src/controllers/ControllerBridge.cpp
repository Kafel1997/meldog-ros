#include "../../include/controllers/ControllerBridge.hpp"

using namespace controller_interface;
using mjbots::pi3hat::CanFrame;


ControllerBridge::ControllerBridge(
     std::unique_ptr<ControllerWrapper> wrapper, 
     const ControllerParameters& params): 
     wrapper_(std::move(wrapper)), params_(params){}
     

void ControllerBridge::make_command(CanFrame& tx_frame, ControllerCommand& command) const
{
    command.position_ = params_.direction_* std::clamp(command.position_,
     params_.position_min_, params_.position_max_) + params_.position_offset_;
    command.velocity_ = params_.direction_* std::clamp(command.velocity_, -params_.velocity_max_, params_.velocity_max_);
    command.torque_ = params_.direction_* std::clamp(command.torque_, -params_.torque_max_, params_.torque_max_);
    wrapper_->command_to_tx_frame(tx_frame, command);
    
}

void ControllerBridge::get_state(const CanFrame& rx_frame, ControllerState& state) const
{
    wrapper_->rx_frame_to_state(rx_frame, state);
    state.position_ = params_.direction_ * state.position_;
    state.velocity_ = params_.direction_ * state.velocity_;
    state.torque_ = params_.direction_ * state.torque_; 
}

void ControllerBridge::make_stop(CanFrame& tx_frame) const
{
    wrapper_->stop_to_tx_frame(tx_frame);
}