#include "main.h"



pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor right_front_top_drive(16);
pros::Motor right_front_bottom_drive(15);
pros::Motor right_back_top_drive(12);
pros::Motor right_back_bottom_drive(1);
pros::Motor left_front_top_drive(19);
pros::Motor left_front_bottom_drive(20);
pros::Motor left_back_top_drive(14);
pros::Motor left_back_bottom_drive(13);
pros::Motor four_bar_left(18);
pros::Motor four_bar_right(17);
pros::Motor ring_manipulator(9);
pros::ADIDigitalOut lift_claw_solenoid(2);
pros::ADIDigitalOut back_claw_solenoid(4);
pros::ADIDigitalOut back_claw_tilt_solenoid(1);

int left_motor_1_speed = 0;
int left_motor_2_speed = 0;
int left_motor_3_speed = 0;
int left_motor_4_speed = 0;
int right_motor_1_speed = 0;
int right_motor_2_speed = 0;
int right_motor_3_speed = 0;
int right_motor_4_speed = 0;

int limit_speed(int speed) {
	if (speed > 127) {
		return 127;
	} else if (speed < -127) {
		return -127;
	} else {
		return speed;
	}
}

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

	four_bar_left.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	four_bar_right.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
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

	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left_stick_y = master.get_analog(ANALOG_LEFT_Y);
		int right_stick_y = master.get_analog(ANALOG_RIGHT_Y);
		int left_stick_x = master.get_analog(ANALOG_LEFT_X);
		int right_stick_x = master.get_analog(ANALOG_RIGHT_X);
		int left_button_1 = master.get_digital(DIGITAL_L1);
		int left_button_2 = master.get_digital(DIGITAL_L2);
		int right_button_1 = master.get_digital(DIGITAL_R1);
		int right_button_2 = master.get_digital(DIGITAL_R2);
		int up_button = master.get_digital(DIGITAL_UP);
		int down_button = master.get_digital(DIGITAL_DOWN);
		int a_button = master.get_digital(DIGITAL_A);
		int b_button = master.get_digital(DIGITAL_B);
		int l1_button = master.get_digital(DIGITAL_L1);
		int x_button = master.get_digital(DIGITAL_X);
		int left_d_button = master.get_digital(DIGITAL_LEFT);
		int right_d_button = master.get_digital(DIGITAL_RIGHT);
		int y_button = master.get_digital(DIGITAL_Y);

		// bool front_claw_open;
		// bool back_claw_open;
		// bool l1_current_state = l1_button;
		// bool l1_previous_state;
		// bool a_current_state = a_button;
		// bool a_previous_state;

		// if (l1_current_state == 1 && l1_previous_state == 0) {
		// 	front_claw_open = !front_claw_open;
		// }

		// if (a_current_state == 1 && a_previous_state == 0) {
		// 	back_claw_open = !back_claw_open;
		// }

		// l1_previous_state = l1_current_state;
		// a_previous_state = a_current_state;

		// right_claw_piston.set_value(front_claw_open);
		// left_claw_piston.set_value(back_claw_open);

		if (down_button) {
			lift_claw_solenoid.set_value(1);
		}
		if (right_d_button) {
			lift_claw_solenoid.set_value(0);
		}

		if (right_button_2) {
			four_bar_right = 127;
			four_bar_left = -127;
		} else if (right_button_1) {
			four_bar_right = -127;
			four_bar_left = 127;
		} else {
			four_bar_right.move_velocity(0);
			four_bar_left.move_velocity(0);
		}

		// if (left_button_1) {
		// 	four_bar_left = -127;
		// } else if (left_button_2) {
		// 	four_bar_left = 127;
		// } else {
		// 	four_bar_left.move_velocity(0);
		// }

		if (left_button_1) {
			ring_manipulator = 127;
		} else if (left_button_2) {
			ring_manipulator = -127;
		} else {
			ring_manipulator.move_velocity(0);
		}

		// Tank Drive
		// left_front_top_drive = left_stick_y;
		// left_front_bottom_drive = -left_stick_y;
		// left_back_top_drive = -left_stick_y;
		// left_back_bottom_drive = left_stick_y;
		// right_front_top_drive = -right_stick_y;
		// right_front_bottom_drive= right_stick_y;
		// right_back_top_drive = right_stick_y;
		// right_back_bottom_drive = -right_stick_y;

		// Split Arcade Drive
		left_motor_1_speed = left_stick_y + right_stick_x;
		left_motor_2_speed = left_stick_y + right_stick_x;
		left_motor_3_speed = left_stick_y + right_stick_x;
		left_motor_4_speed = left_stick_y + right_stick_x;
		right_motor_1_speed = left_stick_y - right_stick_x;
		right_motor_2_speed = left_stick_y - right_stick_x;
		right_motor_3_speed = left_stick_y - right_stick_x;
		right_motor_4_speed = left_stick_y - right_stick_x;

		left_front_top_drive = limit_speed(left_motor_1_speed);
		left_front_bottom_drive = -limit_speed(left_motor_2_speed);
		left_back_top_drive = limit_speed(left_motor_3_speed);
		left_back_bottom_drive = -limit_speed(left_motor_4_speed);
		right_front_top_drive = -limit_speed(right_motor_1_speed);
		right_front_bottom_drive= limit_speed(right_motor_2_speed);
		right_back_top_drive = -limit_speed(right_motor_3_speed);
		right_back_bottom_drive = limit_speed(right_motor_4_speed);

		// Arcade Drive
		// left_motor_1_speed = left_stick_y + left_stick_x;
		// left_motor_2_speed = -(left_stick_y + left_stick_x);
		// left_motor_3_speed = -(left_stick_y + left_stick_x);
		// left_motor_4_speed = left_stick_y + left_stick_x;
		// right_motor_1_speed = -(left_stick_y - left_stick_x);
		// right_motor_2_speed = left_stick_y - left_stick_x;
		// right_motor_3_speed = left_stick_y - left_stick_x;
		// right_motor_4_speed = -(left_stick_y - left_stick_x);

		// left_front_top_drive = limit_speed(left_motor_1_speed);
		// left_front_bottom_drive = limit_speed(left_motor_2_speed);
		// left_back_top_drive = limit_speed(left_motor_3_speed);
		// left_back_bottom_drive = limit_speed(left_motor_4_speed);
		// right_front_top_drive = limit_speed(right_motor_1_speed);
		// right_front_bottom_drive= limit_speed(right_motor_2_speed);
		// right_back_top_drive = limit_speed(right_motor_3_speed);
		// right_back_bottom_drive = limit_speed(right_motor_4_speed);

		pros::delay(20);
	}
}
