from subprocess import Popen, PIPE
from gui.CppException import CppException

class ServerRunner():
    def __init__(self):
        self._p = 0

    def run_server(self):
        command = "communication-bot_server/server"
        self._p = Popen(command, shell=True, stdout=PIPE, stderr=PIPE)
        if self._p.wait():
            raise CppException(self._p.stderr.readline().decode("UTF-8").strip())

    def get_pid(self):
        # -1 process not started
        if self._p != 0:
            return self._p.pid
        else:
            return -1