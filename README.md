# Delay-Based Reservoir Computing

This repository contains code for implementing a Ikeda-type delay-based reservoir computing model. The code is written in C++ and uses the Armadillo library for linear algebra operations.

## Code Files

The repository contains the following code files:

- `delay_based_RC.cpp`: The main code file that implements the delay-based reservoir computing model.
- `reservoirs.h`: Header file containing the definitions of the Ikeda-type delay-based reservoirs that can be used in the model.
    - with the reservoirs.h there is a Base model called dde_reservoir where the Ikeda reservoir model is derived from 

## Usage

To use the code, follow these steps:

1. Clone the repository to your local machine.
2. Install the Armadillo library. (https://arma.sourceforge.net/)
3. Compile the code using the corresponding make file
4. Run the compiled executable with the following command: `./delay_based_RC`
5. Hyperparameter can be set using the command line. To set the delay parameter to 5 and the number of nodes to 100, run the executable with the following command: `./delay_based_RC -delay=5 -num_nodes=100`

## Parameters
# General reservoir parameters
You can set the parameters of the model by passing command line arguments to the executable. The following parameters are available:

- `-delay`: The delay parameter of the reservoir.
- `-num_nodes`: The number of nodes in the reservoir.
- `-theta`: The time constant of the reservoir.
- `-integ_step`: The integration step size.
- `-noise_amp`: The amplitude of the noise added to the reservoir.
- `-seed`: seed for random number generator to draw mask values
- `-pred_step`: set the prediction distance for the time series forecasting 

# Model specific parameters
The Ikeda model is given by the following equation:
$$ \dot{x}(t) = -x(t) +\beta\sin^2(x(t-\tau) + \gamma u(t) + b) + A\chi(t) $$
The parameters can be changed via:
- $\gamma$    →   `-gamma=`
- $b$      →   `-phi=`
- $\beta$    →   `-beta=`
- $A$         →   `-noise_amp=`

## What the code does

- Loads input data from a CSV file named `mackey_glass_tau17.csv` and normalizes it. (Replace this file with your own input data file to change task.)
- Splits the data into training and testing sets.
- Sets reservoir parameters based on command-line arguments.
- Initializes and runs the reservoir with the input data.
- Trains the output layer using the training data.
- Computes and prints the normalized root mean square error (NRMSE) for training and testing predictions.
- Logs the NRMSE values to a file.
- Saves the predicted and actual values of the testing data to separate CSV files.

## Output
The code runs the simulation and save a file called "delay_rc_output.csv" that contains the model and reservoir parameter such as the training and testing performance. Additionally, the code generates two output files: `y_pred.csv` and `y_test.csv`. These files contain the predicted and actual output values, respectively.


## Reproduce paper results 

To reproduce the results for the Mackey-Glass p=17 steps ahead prediction shown in Fig. 3.a) one can run the run_simulations.sh and will receive the performance for the three studied parameter settings: (b=0.0,$gamma$=2) blue, (b=0.5,$gamma$=0.5) green, (b=1.5,$gamma$=1.5) red. As a remark, for ease of use in this script, no iteration over several mask seeds is used whereas in the paper the performance is averaged over 10 different random seeds for the mask.