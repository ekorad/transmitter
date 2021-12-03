//
// File: ert_main.cpp
//
// Code generated for Simulink model 'system_transmitter'.
//
// Model version                  : 1.38
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun Nov  7 12:19:36 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stddef.h>
#include <stdio.h>                // This ert_main.c example uses printf/fflush
#include "system_transmitter.h"        // Model's header file
#include "rtwtypes.h"
#include "builtin_typeid_types.h"

#include "sender.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <iomanip>

static constexpr unsigned long long maxAllowedSampleindex = 5000001;
static unsigned long long requestedMaxStepIndex = maxAllowedSampleindex;
static std::ofstream dbgOut;
static bool debug = false;

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
    static unsigned long long sampleIndex = 0;
    static boolean_T OverrunFlag = false;

    // Disable interrupts here

    // Check for overrun
    if (OverrunFlag) {
        rtmSetErrorStatus(system_transmitter_M, "Overrun");
        return;
    }

    OverrunFlag = true;

    // Save FPU context here (if necessary)
    // Re-enable timer or interrupt here
    // Set model inputs here

    // Step the model for base rate
    system_transmitter_step();

    // Get model outputs here
    DataTransferObject dto{ system_transmitter_Y.xl_out,
        system_transmitter_Y.zl_out, system_transmitter_Y.yr_out };
    if (sendToPeer(dto) == -1)
    {
        perror("sendToPeer");
        exit(EXIT_FAILURE);
    }

    if (debug)
    {
        dbgOut << dto.xl << "," << dto.zl << "," << dto.yr << "\n";
    }

    // Indicate task complete
    OverrunFlag = false;

    // Disable interrupts here
    // Restore FPU context here (if necessary)
    // Enable interrupts here

    sampleIndex++;
    if (sampleIndex == maxAllowedSampleindex)
    {
        rtmSetStopRequested(system_transmitter_M, true);
    }
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, char *argv[])
{
    const std::string dbgFileName = "dbgOut.csv";
    char optChar;
    char* maxIndexArg = nullptr;
    char* portArg = nullptr;
    unsigned port = DEFAULT_PORT;

    while ((optChar = getopt(argc, argv, "dn:p:")) != -1)
    {
        switch (optChar)
        {
            case 'd':
                debug = true;
                dbgOut.open(dbgFileName);
                dbgOut << std::setprecision(17);
                break;
            case 'n':
                if (optarg == nullptr)
                {
                    std::cerr << "-n option requires a valid sample count" << std::endl;
                    return 1;
                }
                else
                {
                    maxIndexArg = optarg;
                }
                break;
            case 'p':
                if (optarg == nullptr)
                {
                    std::cerr << "-p option requires a valid port number" << std::endl;
                    return 1;
                }
                else
                {
                    portArg = optarg;
                }
                break;
            default:
                std::cerr << "Invalid option" << std::endl;
                return 1;
        }
    }

    if (maxIndexArg != nullptr)
    {
        requestedMaxStepIndex = std::stoull(maxIndexArg);
        if (requestedMaxStepIndex > maxAllowedSampleindex)
        {
            std::cerr << "Invalid maximum number of samples" << std::endl;
            return 1;
        }
    }

    if (portArg != nullptr)
    {
        port = std::stoi(portArg);
    }

    if (host(port) == -1)
    {
        std::cerr << "> Connection failed" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "> Connection established" << std::endl;
    }

    // Initialize model
    system_transmitter_initialize();

    // Simulating the model step behavior (in non real-time) to
    //   simulate model behavior at stop time.

    while ((rtmGetErrorStatus(system_transmitter_M) == (NULL)) &&
            !rtmGetStopRequested(system_transmitter_M)) {
        rt_OneStep();
    }

    shutdown();
    std::cout << "> Done, exiting" << std::endl;

    // Disable rt_OneStep() here

    // Terminate model
    system_transmitter_terminate();
    return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
