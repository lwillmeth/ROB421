import pigpio, time, sys

class Motor:
    """Control and interface with the ESC and brushless motor"""

    def __init__(self, pi, pin):
        # Motor components
        self.ESC_MIN        = 1000
        self.ESC_MAX        = 2000
        self.PIN            = pin
        self.pi             = pi

        # Enable ESC output pin
        self.pi.set_mode(self.PIN, pigpio.OUTPUT)

        # Allow the ESC to 'wake up' by sending minimum value
        self.set(self.ESC_MIN)

    # def set_ESC_range(self, low=800, high=1800):
    #     '''Configure input range on ESC, MUST run while ESC is starting!'''
    #     'Times are arbitrary, but these take a few seconds.'
    #     self.pi.set_servo_pulsewidth(self.PIN, low)
    #     time.sleep(10)
    #     self.pi.set_servo_pulsewidth(self.PIN, high)
    #     time.sleep(3)
    #     self.pi.set_servo_pulsewidth(self.PIN, 0)     # off
    #     time.sleep(2)
    #     return True

    def set(self, speed):
        '''Set the motor speed to a PWM value'''
        if self.ESC_MIN <= speed <= self.ESC_MAX:
            self.pi.set_servo_pulsewidth(self.PIN, speed)
            return True
        print("Bad value")
        return False

def demo():
    piggy = pigpio.pi()
    m1 = Motor(pi=piggy, pin=18)
    m2 = Motor(pi=piggy, pin=13)

    print("Waking up ESC..")
    piggy.set_servo_pulsewidth(18, 1000)
    time.sleep(5)

    print("Running motors at max speed..")
    m1.set(2000)
    m2.set(2000)
    input("Press ENTER to stop demo.")
    print("Halting motors..")
    m1.set(1000)
    m2.set(1000)

if __name__ == "__main__":
    demo()
