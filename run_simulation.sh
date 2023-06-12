#!/bin/bash

executable="./delay_based_RC"

# Default options
options=(
  "-seed=0"
  "-pred_steps=17"
  "-beta=1.6"
  "-noise_amp=0.005"
  "-num_nodes=500"
  "-theta=0.1"
  "-delay=55"
)

# Run with different values of phi and gamma
run_simulation() {
  local phi=$1
  local gamma=$2
  $executable "${options[@]}" "-phi=$phi" "-gamma=$gamma"
}

# Example runs
run_simulation 0.0 2.0
run_simulation 0.5 0.5
run_simulation 1.0 1.5
