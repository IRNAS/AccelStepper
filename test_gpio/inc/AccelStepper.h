/*
 * AccelStepper.h
 *
 *  Created on: 27. jun. 2016
 *      Author: user
 */

#ifndef ACCELSTEPPER_H_
#define ACCELSTEPPER_H_

// !! uncomment for debug
//#define ACCEL_STEPPER_DEBUG

/// \brief Symbolic names for number of pins.
/// Use this in the pins argument the AccelStepper constructor to
/// provide a symbolic name for the number of pins
/// to use.
typedef enum
{
	FUNCTION  = 0, ///< Use the functional interface, implementing your own driver functions (internal use only)
	DRIVER    = 1, ///< Stepper Driver, 2 driver pins required
	FULL2WIRE = 2, ///< 2 wire stepper, 2 motor pins required
	FULL3WIRE = 3, ///< 3 wire stepper, such as HDD spindle, 3 motor pins required
	FULL4WIRE = 4, ///< 4 wire full stepper, 4 motor pins required
	HALF3WIRE = 6, ///< 3 wire half stepper, such as HDD spindle, 3 motor pins required
	HALF4WIRE = 8  ///< 4 wire half stepper, 4 motor pins required
} MotorInterface_t;


typedef struct
{
	/// Number of pins on the stepper motor. Permits 2 or 4. 2 pins is a
	/// bipolar, and 4 pins is a unipolar.
	uint8_t _interface; // 0, 1, 2, 4, 8, See MotorInterface_t

	/// Pin number assignments for the 2 or 4 pins required to interface to the
	/// stepper motor or driver
	uint16_t _pin[4];
	GPIO_TypeDef* _GPIOxPin[4];

	/// Whether the _pins is inverted or not
	uint8_t _pinInverted[4]; // bool

	/// The current absolution position in steps.
	long _currentPos; // Steps

	/// The target position in steps. The AccelStepper library will move the
	/// motor from the _currentPos to the _targetPos, taking into account the
	/// max speed, acceleration and deceleration
	long _targetPos; // Steps

	/// The current motos speed in steps per second
	/// Positive is clockwise
	float _speed; // Steps per second

	/// The maximum permitted speed in steps per second. Must be > 0.
	float _maxSpeed;

	/// The acceleration to use to accelerate or decelerate the motor in steps
	/// per second per second. Must be > 0
	float _acceleration;
	float _sqrt_twoa; // Precomputed sqrt(2*_acceleration)

	/// The current interval between steps in microseconds.
	/// 0 means the motor is currently stopped with _speed == 0
	unsigned long _stepInterval;

	/// The last step time in microseconds
	unsigned long _lastStepTime;

	/// The minimum allowed pulse width in microseconds
	unsigned int _minPulseWidth;

	/// Is the direction pin inverted?
	///uint8_t _dirInverted; // bool /// Moved to _pinInverted[1]

	/// Is the step pin inverted?
	///uint8_t _stepInverted; // bool /// Moved to _pinInverted[0]

	/// Is the enable pin inverted?
	uint8_t _enableInverted; // bool

	/// Enable pin for stepper driver, or 0xFFFF if unused.
	uint16_t _enablePin;
	GPIO_TypeDef* _GPIOxEnablePin;

	/// The pointer to a forward-step procedure
	void (*_forward)();

	/// The pointer to a backward-step procedure
	void (*_backward)();

	/// The step counter for speed calculations
	long _n;

	/// Initial step size in microseconds
	float _c0;

	/// Last step size in microseconds
	float _cn;

	/// Min step size in microseconds based on maxSpeed
	float _cmin; // at max speed

	/// Current direction motor is spinning in
	uint8_t _direction; // 1 == CW // possible values 0 or 1
} Stepper_t;


//Constrains a number to be within a range defined by minimum and maximum.
extern float constrain(float value, float minimum, float maximum);


/// Constructor. You can have multiple simultaneous steppers, all moving
/// at different speeds and accelerations, provided you call their run()
/// functions at frequent enough intervals. Current Position is set to 0, target
/// position is set to 0. MaxSpeed and Acceleration default to 1.0.
/// The motor pins will be initialised to OUTPUT mode during the
/// constructor by a call to enableOutputs().
/// \param[in] interface Number of pins to interface to. 1, 2, 4 or 8 are
/// supported, but it is preferred to use the \ref MotorInterface_t symbolic names.
/// AccelStepper::DRIVER (1) means a stepper driver (with Step and Direction pins).
/// If an enable line is also needed, call setEnablePin() after construction.
/// You may also invert the pins using setPinsInverted().
/// AccelStepper::FULL2WIRE (2) means a 2 wire stepper (2 pins required).
/// AccelStepper::FULL3WIRE (3) means a 3 wire stepper, such as HDD spindle (3 pins required).
/// AccelStepper::FULL4WIRE (4) means a 4 wire stepper (4 pins required).
/// AccelStepper::HALF3WIRE (6) means a 3 wire half stepper, such as HDD spindle (3 pins required)
/// AccelStepper::HALF4WIRE (8) means a 4 wire half stepper (4 pins required)
/// Defaults to AccelStepper::FULL4WIRE (4) pins.
/// \param[in] pin1 GPIO pin number for motor pin 1. Defaults
/// to pin 2. For a AccelStepper::DRIVER (interface==1),
/// this is the Step input to the driver. Low to high transition means to step)
/// \param[in] pin2 GPIO pin number for motor pin 2. Defaults
/// to pin 3. For a AccelStepper::DRIVER (interface==1),
/// this is the Direction input the driver. High means forward.
/// \param[in] pin3 GPIO pin number for motor pin 3. Defaults
/// to pin 4.
/// \param[in] pin4 GPIO pin number for motor pin 4. Defaults
/// to pin 5.
/// \param[in] enable If this is true (the default), enableOutputs() will be called to enable
/// the output pins at construction time.
extern void InitStepper(Stepper_t* motor, uint8_t interface, uint16_t pin1, GPIO_TypeDef* GPIOxPin1, uint16_t pin2, GPIO_TypeDef* GPIOxPin2, uint16_t pin3, GPIO_TypeDef* GPIOxPin3, uint16_t pin4, GPIO_TypeDef* GPIOxPin4, uint8_t enable);


/// Alternate Constructor which will call your own functions for forward and backward steps.
/// You can have multiple simultaneous steppers, all moving
/// at different speeds and accelerations, provided you call their run()
/// functions at frequent enough intervals. Current Position is set to 0, target
/// position is set to 0. MaxSpeed and Acceleration default to 1.0.
/// Any motor initialization should happen before hand, no pins are used or initialized.
/// \param[in] forward void-returning procedure that will make a forward step
/// \param[in] backward void-returning procedure that will make a backward step
extern void InitStepperFunct(Stepper_t* motor, void (*forward)(), void (*backward)());


/// Set the target position. The run() function will try to move the motor (at most one step per call)
/// from the current position to the target position set by the most
/// recent call to this function. Caution: moveTo() also recalculates the speed for the next step.
/// If you are trying to use constant speed movements, you should call setSpeed() after calling moveTo().
/// \param[in] absolute The desired absolute position. Negative is
/// anticlockwise from the 0 position.
extern void moveTo(Stepper_t* motor, long absolute);


/// Set the target position relative to the current position
/// \param[in] relative The desired position relative to the current position. Negative is
/// anticlockwise from the current position.
extern void move(Stepper_t* motor, long relative);


/// Poll the motor and step it if a step is due, implementing
/// accelerations and decelerations to acheive the target position. You must call this as
/// frequently as possible, but at least once per minimum step time interval,
/// preferably in your main loop. Note that each call to run() will make at most one step, and then only when a step is due,
/// based on the current speed and the time since the last step.
/// \return true if the motor is still running to the target position.
extern uint8_t run(Stepper_t* motor);


/// Poll the motor and step it if a step is due, implementing a constant
/// speed as set by the most recent call to setSpeed(). You must call this as
/// frequently as possible, but at least once per step interval,
/// \return true if the motor was stepped.
extern uint8_t runSpeed(Stepper_t* motor);


/// Sets the maximum permitted speed. The run() function will accelerate
/// up to the speed set by this function.
/// Caution: the maximum speed achievable depends on your processor and clock speed.
/// \param[in] speed The desired maximum speed in steps per second. Must
/// be > 0. Caution: Speeds that exceed the maximum speed supported by the processor may
/// Result in non-linear accelerations and decelerations.
extern void setMaxSpeed(Stepper_t* motor, float speed);

/// returns the maximum speed configured for this stepper
/// that was previously set by setMaxSpeed();
/// \return The currently configured maximum speed
extern float maxSpeed(Stepper_t* motor);

/// Sets the acceleration/deceleration rate.
/// \param[in] acceleration The desired acceleration in steps per second
/// per second. Must be > 0.0. This is an expensive call since it requires a square
/// root to be calculated. Dont call more ofthen than needed
extern void setAcceleration(Stepper_t* motor, float acceleration);

/// Sets the desired constant speed for use with runSpeed().
/// \param[in] speed The desired constant speed in steps per
/// second. Positive is clockwise. Speeds of more than 1000 steps per
/// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
/// once per hour, approximately. Speed accuracy depends on the crystal.
/// Jitter depends on how frequently you call the runSpeed() function.
extern void setSpeed(Stepper_t* motor, float speed);

/// The most recently set speed
/// \return the most recent speed in steps per second
extern float speed(Stepper_t* motor);

/// The distance from the current position to the target position.
/// \return the distance from the current position to the target position
/// in steps. Positive is clockwise from the current position.
extern long distanceToGo(Stepper_t* motor);

/// The most recently set target position.
/// \return the target position
/// in steps. Positive is clockwise from the 0 position.
extern long targetPosition(Stepper_t* motor);

/// The currently motor position.
/// \return the current motor position
/// in steps. Positive is clockwise from the 0 position.
extern long currentPosition(Stepper_t* motor);

/// Resets the current position of the motor, so that wherever the motor
/// happens to be right now is considered to be the new 0 position. Useful
/// for setting a zero position on a stepper after an initial hardware
/// positioning move.
/// Has the side effect of setting the current motor speed to 0.
/// \param[in] position The position in steps of wherever the motor
/// happens to be right now.
extern void setCurrentPosition(Stepper_t* motor, long position);

/// Moves the motor (with acceleration/deceleration)
/// to the target position and blocks until it is at
/// position. Dont use this in event loops, since it blocks.
extern void runToPosition(Stepper_t* motor);

/// Runs at the currently selected speed until the target position is reached
/// Does not implement accelerations.
/// \return true if it stepped
extern uint8_t runSpeedToPosition(Stepper_t* motor);

/// Moves the motor (with acceleration/deceleration)
/// to the new target position and blocks until it is at
/// position. Dont use this in event loops, since it blocks.
/// \param[in] position The new target position.
extern void runToNewPosition(Stepper_t* motor, long position);

/// Sets a new target position that causes the stepper
/// to stop as quickly as possible, using the current speed and acceleration parameters.
extern void stop(Stepper_t* motor);

/// Disable motor pin outputs by setting them all LOW
/// Depending on the design of your electronics this may turn off
/// the power to the motor coils, saving power.
/// This is useful to support low power mode: disable the outputs
/// during sleep and then reenable with enableOutputs() before stepping
/// again.
/// If the enable Pin is defined, sets it to OUTPUT mode and clears the pin to disabled.
extern void disableOutputs(Stepper_t* motor);

/// Enable motor pin outputs by setting the motor pins to OUTPUT
/// mode. Called automatically by the constructor.
/// If the enable Pin is defined, sets it to OUTPUT mode and sets the pin to enabled.
extern void enableOutputs(Stepper_t* motor);

/// Sets the minimum pulse width allowed by the stepper driver. The minimum practical pulse width is
/// approximately 20 microseconds. Times less than 20 microseconds
/// will usually result in 20 microseconds or so.
/// \param[in] minWidth The minimum pulse width in microseconds.
extern void setMinPulseWidth(Stepper_t* motor, unsigned int minWidth);

/// Sets the enable pin number for stepper drivers.
/// 0xFF indicates unused (default).
/// Otherwise, if a pin is set, the pin will be turned on when
/// enableOutputs() is called and switched off when disableOutputs()
/// is called.
/// \param[in] enablePin GPIO pin number for motor enable
/// \sa setPinsInverted
extern void setEnablePin(Stepper_t* motor, uint16_t enablePin, GPIO_TypeDef* GPIOxEnablePin);

/// Sets the inversion for stepper driver pins
/// \param[in] directionInvert True for inverted direction pin, false for non-inverted
/// \param[in] stepInvert      True for inverted step pin, false for non-inverted
/// \param[in] enableInvert    True for inverted enable pin, false (default) for non-inverted
extern void setPinsInvertedStpDir(Stepper_t* motor, uint8_t directionInvert, uint8_t stepInvert, uint8_t enableInvert);

/// Sets the inversion for 2, 3 and 4 wire stepper pins
/// \param[in] pin1Invert True for inverted pin1, false for non-inverted
/// \param[in] pin2Invert True for inverted pin2, false for non-inverted
/// \param[in] pin3Invert True for inverted pin3, false for non-inverted
/// \param[in] pin4Invert True for inverted pin4, false for non-inverted
/// \param[in] enableInvert    True for inverted enable pin, false (default) for non-inverted
extern void setPinsInverted(Stepper_t* motor, uint8_t pin1Invert, uint8_t pin2Invert, uint8_t pin3Invert, uint8_t pin4Invert, uint8_t enableInvert);

/// Checks to see if the motor is currently running to a target
/// \return true if the speed is not zero or not at the target position
extern uint8_t isRunning(Stepper_t* motor);

/// \brief Direction indicator
/// Symbolic names for the direction the motor is turning
typedef enum
{
	DIRECTION_CCW = 0,  ///< Clockwise
	DIRECTION_CW  = 1   ///< Counter-Clockwise
} Direction;

/// Forces the library to compute a new instantaneous speed and set that as
/// the current speed. It is called by
/// the library:
/// \li  after each step
/// \li  after change to maxSpeed through setMaxSpeed()
/// \li  after change to acceleration through setAcceleration()
/// \li  after change to target position (relative or absolute) through
/// move() or moveTo()
extern void computeNewSpeed(Stepper_t* motor);

/// Low level function to set the motor output pins
/// bit 0 of the mask corresponds to _pin[0]
/// bit 1 of the mask corresponds to _pin[1]
/// You can override this to impment, for example serial chip output insted of using the
/// output pins directly
extern void setOutputPins(Stepper_t* motor, uint8_t mask);

/// Called to execute a step. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default calls step1(), step2(), step4() or step8() depending on the
/// number of pins defined for the stepper.
/// \param[in] step The current step phase number (0 to 7)
extern void step(Stepper_t* motor, long step);

/// Called to execute a step using stepper functions (pins = 0) Only called when a new step is
/// required. Calls _forward() or _backward() to perform the step
/// \param[in] step The current step phase number (0 to 7)
extern void step0(Stepper_t* motor, long step);

/// Called to execute a step on a stepper driver (ie where pins == 1). Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of Step pin1 to step,
/// and sets the output of _pin2 to the desired direction. The Step pin (_pin1) is pulsed for 1 microsecond
/// which is the minimum STEP pulse width for the 3967 driver.
/// \param[in] step The current step phase number (0 to 7)
extern void step1(Stepper_t* motor, long step);

/// Called to execute a step on a 2 pin motor. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of pin1 and pin2
/// \param[in] step The current step phase number (0 to 7)
extern void step2(Stepper_t* motor, long step);

/// Called to execute a step on a 3 pin motor, such as HDD spindle. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of pin1, pin2,
/// pin3
/// \param[in] step The current step phase number (0 to 7)
extern void step3(Stepper_t* motor, long step);

/// Called to execute a step on a 4 pin motor. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of pin1, pin2,
/// pin3, pin4.
/// \param[in] step The current step phase number (0 to 7)
extern void step4(Stepper_t* motor, long step);

/// Called to execute a step on a 3 pin motor, such as HDD spindle. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of pin1, pin2,
/// pin3
/// \param[in] step The current step phase number (0 to 7)
extern void step6(Stepper_t* motor, long step);

/// Called to execute a step on a 4 pin half-steper motor. Only called when a new step is
/// required. Subclasses may override to implement new stepping
/// interfaces. The default sets or clears the outputs of pin1, pin2,
/// pin3, pin4.
/// \param[in] step The current step phase number (0 to 7)
extern void step8(Stepper_t* motor, long step);

#endif /* ACCELSTEPPER_H_ */
