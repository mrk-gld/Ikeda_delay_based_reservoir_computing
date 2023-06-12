/*

written by: Mirko Goldmann
date: 2023-05-15

*/

#include <armadillo>
#include <cmath>

using namespace arma;
using namespace std;

#define LONG_LINE "______________________"

// noise generator
mt19937 gen;
normal_distribution<float> noise;

// delay-based RC Base struct, all other delay-based RCs inherit from this and can override its functionality
template <typename T>
struct dde_reservoir
{

    float delay = 80.f;
    int num_nodes = 50;
    float theta = 1.4f; // time per virtual node
    float integ_step = 0.01;
    float noise_amp = 1e-3f;
    string name = "dde_reservoir";

    T z_t;
    vector<T> z_tau;

    void init_delay(T x_0)
    {
        int steps_per_delay = int(delay / integ_step);
        z_t = x_0;
        z_tau = vector<float>(steps_per_delay);
        for (int i = 0; i < steps_per_delay; i++)
        {
            z_tau[i] =  x_0 + noise_amp * noise(gen);
        }
    };

    virtual void init_delay()
    {
        
    };

    virtual float readout() { return 0.0; };

    virtual T dde_equation(T z_t, T z_tau, float u_t) { return -z_t; };

    virtual void set_parameters(map<string, float> params){};

    virtual void print_parameters()
    {
        cout << endl
             << endl
             << LONG_LINE << endl;
        cout << "General RC parameters:" << endl;
        cout << "delay = " << delay << endl;
        cout << "num_nodes = " << num_nodes << endl;
        cout << "theta = " << theta << endl;
        cout << "input time = " << theta * num_nodes << endl;
        cout << "integ_step = " << integ_step << endl;
        cout << "noise_amp = " << noise_amp << endl;
        cout << LONG_LINE << endl;
    };

    void set_rc_parameters(map<string, float> params)
    {
        // check if parameter is in map
        if (params.find("delay") != params.end())
        {
            delay = params["delay"];
        }
        if (params.find("num_nodes") != params.end())
        {
            num_nodes = int(params["num_nodes"]);
        }
        if (params.find("theta") != params.end())
        {
            theta = params["theta"];
        }
        if (params.find("integ_step") != params.end())
        {
            integ_step = params["integ_step"];
        }
        if (params.find("noise_amp") != params.end())
        {
            noise_amp = params["noise_amp"];
        }
    };

    virtual string csv_header()
    {
        stringstream ss;
        ss << endl;
        return ss.str();
    }

    void euler_maruyama(float u_t)
    {

        T dzdt = dde_equation(z_t, z_tau[0], u_t);

        // generate float noise term
        T real_noise = noise(gen);
        T noise_term = noise_amp * real_noise;

        // in case state variable is complex, generate complex noise term
        if constexpr (std::is_same_v<T, std::complex<float>>)
        {
            std::complex<float> complex_noise(noise(gen), noise(gen));
            noise_term = noise_amp * complex_noise;
        }

        T new_zk1 = z_t + integ_step * dzdt + noise_term * sqrt(integ_step);

        z_t = new_zk1;

        // update delay line
        z_tau.push_back(new_zk1);
        z_tau.erase(z_tau.begin());
    }

};

// Ikeda model inhereting from dde_reservoir
struct Ikeda : dde_reservoir<float>
{
    float beta = 1.6;
    float gamma = 0.9;
    float epsilon = 1.f;
    float phi = 0.2;

    Ikeda()
    {
        name = "ikeda";
    }

    void set_parameters(map<string, float> params)
    {
        // check if parameter is in map
        if (params.find("beta") != params.end())
        {
            beta = params["beta"];
        }
        if (params.find("gamma") != params.end())
        {
            gamma = params["gamma"];
        }
        if (params.find("epsilon") != params.end())
        {
            epsilon = params["epsilon"];
        }
        if (params.find("phi") != params.end())
        {
            phi = params["phi"];
        }
        set_rc_parameters(params);
    }

    void print_parameters()
    {
        cout << endl
             << endl
             << LONG_LINE << endl;
        cout << "Ikeda RC parameters:" << endl;
        cout << "beta = " << beta << endl;
        cout << "gamma = " << gamma << endl;
        cout << "epsilon = " << epsilon << endl;
        cout << "phi = " << phi << endl;
        cout << LONG_LINE << endl;
        dde_reservoir::print_parameters();
    }

    void init_delay() override
    {
        int steps_per_delay = int(delay / integ_step);
        z_t = 0.1f;
        z_tau = vector<float>(steps_per_delay, 0.1f);
    }

    float readout()
    {
        return z_t;
    }

    // Runge Kutte 4th order method for mackey glass oscillator
    float dde_equation(float z_t, float z_tau, float u_t)
    {
        float sin_term = sin(z_tau + gamma * u_t + phi);
        return -epsilon * z_t + beta * sin_term * sin_term;
    }

    string csv_header()
    {
        stringstream ss;
        ss << "beta,gamma,epsilon,phi" << endl;
        ss << beta << "," << gamma << "," << epsilon << "," << phi << endl;
        return ss.str();
    }
};