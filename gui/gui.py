from flask import Flask, request, render_template
app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def get_data():
    if request.method == 'POST':
        file = open("conf.txt","w")
        file.write(request.form['ip_address'])
        file.write("\n")
        file.write(request.form['port'])
        file.write("\n")
        file.write(request.form['conn_number'])
        file.write("\n")
        file.write(request.form['req_number'])
        file.write("\n")
        file.write(request.form['requ'])
        file.close()

        return render_template('good.html')
    else:
        return render_template('main.html')

if __name__ == '__main__':
    app.run()
