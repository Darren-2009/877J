/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Darren Gola                                                     */
/*    Created:      10/16/2026, 4:51:31 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
brain Brain;
controller Controller;
// Drivetrain
motor mFrontL = motor(PORT1,ratio6_1,true);
motor mBackL = motor(PORT2, ratio6_1, false);
motor mFrontR = motor(PORT3, ratio6_1, false);
motor mBackR = motor(PORT4, ratio6_1, true);
motor_group gLeft = motor_group(mFrontL,mBackL);
motor_group gRight = motor_group(mFrontR,mBackR);
// Banshee
motor mClaw = motor(PORT5, ratio18_1, true);
motor mSwerve = motor(PORT8, ratio18_1, true);
// Lift
motor mWinchL = motor(PORT6, ratio18_1, true);
motor mWinchR = motor(PORT7, ratio18_1, false);
motor_group gWinch = motor_group(mWinchL,mWinchR);
// Intake
motor mIntake = motor(PORT9, ratio18_1, true);

/*---------------------------------------------------------------------------*/
/*                                   PID                                     */
/*                                                                           */
/*  Proportional, Integral, Derivative control                               */
/*  Decrease the values of the PID constants to improve stability            */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  mClaw.setStopping(hold);
  gWinch.setStopping(hold);
  mSwerve.setPosition(0, turns);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  gLeft.setVelocity(50, percent);
  gRight.setVelocity(50, percent);
  gLeft.spinTo(8, turns, false);
  gRight.spinTo(8, turns, true);

  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void clawScore() {
  mClaw.setPosition(0, turns);
  mClaw.setVelocity(100, percent);
  mClaw.spinTo(1, turns, false);
  mSwerve.spinTo(0.3325, turns, true);
  mSwerve.spinTo(0.3325, turns, false);
}
void clawIntake() {
  mSwerve.spinTo(0, turns, true);
}
void clawBanshee() {
  mSwerve.spinTo(0.3325, turns, true);
}
void usercontrol() {
  // User control code here, inside the loop

  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // Drivetrain
    gLeft.setVelocity(Controller.Axis3.position()+Controller.Axis1.position(), percent);
    gRight.setVelocity(Controller.Axis3.position()-Controller.Axis1.position(), percent);
    gRight.spin(forward);
    gLeft.spin(forward);
    // Claw Score
      Controller.ButtonX.pressed(clawScore);
    // Claw Intake
      Controller.ButtonDown.pressed(clawIntake);
    // Claw
    if(Controller.ButtonL1.pressing()) {
      mClaw.spin(forward);
    } else if (Controller.ButtonL2.pressing()) {
      mClaw.spin(reverse);
    }  else{
      mClaw.stop();
    }
    // Winch
    if(Controller.ButtonR1.pressing()) {
      gWinch.spin(forward);
    } else if (Controller.ButtonR2.pressing()) {
      gWinch.spin(reverse);
    }  else{
      gWinch.stop();
    }
    // Intake
    if(Controller.ButtonUp.pressing()) {
      mIntake.spin(forward);
    } else if (Controller.ButtonDown.pressing()) {
      mIntake.spin(reverse);
    }  else{
      mIntake.stop();
    }
    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
