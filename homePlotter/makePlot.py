#!/usr/bin/env python
import pandas as pd
from bokeh.plotting import save, figure, show, output_file, vplot
from bokeh.embed import components
from bokeh.models import Range1d
import MySQLdb
import subprocess
import datetime


# Open database connection
db = MySQLdb.connect("localhost","esp8266","secretpwd","esp8266" )

# prepare a cursor object using cursor() method
cursor = db.cursor()

# execute SQL query using execute() method.
sqlQuery = "SELECT * FROM DataTable;"

df = pd.read_sql(sqlQuery, db)

df = df[df.temperature != 0]

# disconnect from server
db.close()

startTime = datetime.datetime(2016, 7, 30, 7, 6, 5)
endTime = datetime.datetime(2016, 9, 30, 7, 6, 5)

titText = 'Last updated at ' + str(df.logdate.tail(1)) + ' - ' + str(df.temperature.tail(1)) + ' C, ' + str(df.humidity.tail(1)) + ' % humidity'

p1 = figure(title=titText, height=400, width=800, x_axis_type="datetime")
p1.line(df.logdate, df.humidity, color='#183048', legend='Humidity', line_width=3)
p1.xaxis.axis_label = "Date/Time"
p1.yaxis.axis_label = "Humidity (%)"
p1.x_range=Range1d(startTime,endTime)

p2 = figure(tools='', height=400, width=800, x_range=p1.x_range, x_axis_type="datetime")
p2.line(df.logdate, df.temperature, color='#90C0C0', legend='Temperature', line_width=3)
p2.xaxis.axis_label = "Date/Time"
p2.yaxis.axis_label = "Temperature (C)"
p2.y_range=Range1d(18,28)
p2.logo = None
p2.toolbar_location = None

p3 = figure(tools='', height=400, width=800, x_range=p1.x_range, x_axis_type="datetime")
p3.line(df.logdate, df.sound, color='#F03000', legend='Sound', line_width=3)
p3.xaxis.axis_label = "Date/Time"
p3.yaxis.axis_label = "Sound"
p3.logo = None
p3.toolbar_location = None

output_file("tempPlots.html", title="Temp/Humitidy plots")
#save(vplot(p1,p2,p3))
save(vplot(p1,p2))

subprocess.call("cp ./tempPlots.html /var/www/html/", shell=True)

