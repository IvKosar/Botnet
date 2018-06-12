from subprocess import Popen, PIPE
from gui.CppException import CppException

def run_server():
    command = "communication-bot_server/server"
    p = Popen(command, shell=True, stdout=PIPE, stderr=PIPE)
    if p.wait():
        raise CppException(p.stderr.readline().decode("UTF-8").strip())
