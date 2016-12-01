from threading import *
from time import sleep
import os

class PingPong:
    def __init__(self):
        self.m = Lock()
        self.turn = True
        self.cond = Condition(self.m)
    def ping(self):
        while True:
            with self.m:
                while not self.turn:
                    self.cond.wait()
                print "ping is here"
                self.turn = False
                self.cond.notify()
    def pong(self):
        while True:
            with self.m:
                while self.turn:
                    self.cond.wait()
                print "pong is here"
                self.turn = True
                self.cond.notify()



if __name__ == "__main__":
    # Create threads
    p = PingPong()
    tping = Thread(target=PingPong.ping, args=(p,))
    tpong = Thread(target=PingPong.pong, args=(p,))

    # Start threads
    tping.start()
    tpong.start()

    # Allow CTRL + C to exit the program
    try:
        while True: sleep(0.1)
    except (KeyboardInterrupt, SystemExit):
        os._exit(0)
