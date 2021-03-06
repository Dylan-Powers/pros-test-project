#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor right_motor_1(1);
	pros::Motor right_motor_2(2);
	pros::Motor right_motor_3(3);
	pros::Motor right_motor_4(4);
	pros::Motor left_motor_1(5);
	pros::Motor left_motor_2(6);
	pros::Motor left_motor_3(7);
	pros::Motor left_motor_4(8);	
	pros::Motor mobile_goal_claw(9);
	mobile_goal_claw.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Motor manipulator_left_wrist(20);
	manipulator_left_wrist.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::Motor manipulator_right_wrist(11);
	manipulator_right_wrist.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	pros::ADIDigitalOut grabber_solenoid(8);

	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left_stick_y = master.get_analog(ANALOG_LEFT_Y);
		int right_stick_y = master.get_analog(ANALOG_RIGHT_Y);
		int left_button_1 = master.get_digital(DIGITAL_L1);
		int left_button_2 = master.get_digital(DIGITAL_L2);
		int right_button_1 = master.get_digital(DIGITAL_R1);
		int right_button_2 = master.get_digital(DIGITAL_R2);
		int up_button = master.get_digital(DIGITAL_UP);
		int down_button = master.get_digital(DIGITAL_DOWN);
		int a_button = master.get_digital(DIGITAL_A);
		int b_button = master.get_digital(DIGITAL_B);

		bool grabber_open;
		bool current_state = a_button;
		bool previous_state;

		left_motor_1 = left_stick_y;
		left_motor_2 = -left_stick_y;
		left_motor_3 = -left_stick_y;
		left_motor_4 = left_stick_y;
		right_motor_1 = -right_stick_y;
		right_motor_2 = right_stick_y;
		right_motor_3 = right_stick_y;
		right_motor_4 = -right_stick_y;

		// Not technically position control, but it should put the manipulator in a "brake mode"
		if (left_button_1) {
			mobile_goal_claw = 127;
		} else if (left_button_2) {
			mobile_goal_claw = -127;
		} else {
			mobile_goal_claw.move_velocity(0);
		}

		if (current_state == 1 && previous_state == 0) {
			grabber_open = !grabber_open;
		}

		previous_state = current_state;

		// TODO: make this a single button to toggle between open and close
		// if (a_button) {
		// 	grabber_open = true;
		// }
		// if (b_button) {
		// 	grabber_open = false;
		// }

		grabber_solenoid.set_value(grabber_open);

		if (right_button_1) {
			manipulator_left_wrist = 127;
			manipulator_right_wrist = -127;
		} else if (right_button_2) {
			manipulator_left_wrist = -127;
			manipulator_right_wrist = 127;
		} else {
			manipulator_left_wrist.move_velocity(0);
			manipulator_right_wrist.move_velocity(0);
		}

		pros::delay(20);
	}
}
