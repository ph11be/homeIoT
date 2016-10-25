''' Bokeh application for plotting the environmental parameters at home


'''
import numpy as np
import pandas as pd
import MySQLdb
import subprocess
import datetime

from bokeh.io import curdoc, show
from bokeh.layouts import row, widgetbox
from bokeh.models import ColumnDataSource
from bokeh.models.widgets import Slider, TextInput, Button, DatePicker, RadioButtonGroup
from bokeh.plotting import figure

sensorLocations = ['Living Room', 'Nursery/Kitchen', 'Dining Room']
plotPalette = ['#90C0C0', '#66ffcc', '#BB472E', '', '']
sensorList = ['temperature','humidity','lux']

def grabData(sensor, start_time, end_time):
    # Open database connection
    print str(start_time)
    print str(end_time)
    db = MySQLdb.connect("localhost","esp8266","secretpwd","esp8266" )

    # prepare a cursor object using cursor() method
    cursor = db.cursor()

    # execute SQL query using execute() method.

    dTable = ['DataTable', 'nodemcu1', 'nodemcu2']
    sensors = ['temperature', 'humidity', 'lux']
    
    if sensor not in sensors:
        sensor = 'temperature'

    df = {}

    for i in range(0,3):
        sqlQuery = "SELECT logdate, " + sensor + " FROM " + dTable[i] + \
                  " WHERE (logdate BETWEEN '" + start_time.strftime("%Y-%m-%d") + "' AND '" + end_time.strftime("%Y-%m-%d")  + "') AND " + \
                  sensor + " <> 0 AND " + sensor + "< 100;"

        print sqlQuery
        df[i] = pd.read_sql(sqlQuery, db)

    # disconnect from server
    db.close()
  
    return df


#------------------------------------------------------------------------------------
# Set up widgets
#------------------------------------------------------------------------------------
text = TextInput(title="title", value='my sine wave')
refreshButton = Button(label="Refresh", button_type="default")

pr = RadioButtonGroup(labels=sensorList, active=0)

startDate = DatePicker(title='Start Date', min_date=datetime.datetime.now()-datetime.timedelta(days=100), max_date=datetime.datetime.now()+datetime.timedelta(days=1), value=datetime.datetime.now()-datetime.timedelta(days=7))
endDate = DatePicker(title='End date', min_date=datetime.datetime.now()-datetime.timedelta(days=100), max_date=datetime.datetime.now()+datetime.timedelta(days=1), value=datetime.datetime.now()+datetime.timedelta(days=1)) 

print 'Start date: ' + str(startDate.value) + '  ' + str(type(startDate.value))
print 'End date: ' + str(endDate.value) + '  ' + str(type(endDate.value))

sTime = startDate.value
eTime = endDate.value


#------------------------------------------------------------------------------------
# Set up data
#------------------------------------------------------------------------------------
df = grabData(sensorList[pr.active], startDate.value, endDate.value)

#------------------------------------------------------------------------------------
# Set up plot
#------------------------------------------------------------------------------------

def plotData(df):
    global pr, sensorList, sensorLocations, plotPalette

    titText = 'Last updated at ' + str(df[0].logdate.tail(1))
    plot = figure(title=titText, height=600, width=700, x_axis_type="datetime")

    numDatasets = len(df)

    print 'Number of datasets to plot: ' + str(numDatasets)

    for i in range(0, numDatasets):
        print df[i].head()
        plot.line(df[i].logdate, df[i][sensorList[pr.active]], color=plotPalette[i], legend=sensorLocations[i], line_width=3)
        print "Plotting line"
    return plot

def updateData():
    global pr, inputs, sensorList, startDate, endDate
    print "Button has been pressed"
    df = grabData(sensorList[pr.active], startDate.value, endDate.value)
    print df[0].head()
    plot = plotData(df)
    layout.children[1] = plotData(df)


#------------------------------------------------------------------------------------
# Set up callbacks
#------------------------------------------------------------------------------------
def update_title(attrname, old, new):
    plot.title.text = text.value

def updateStart(attrname, old, new):
    sTime = startDate.value
    print str(sTime)

def updateEnd(attrname, old, new):
    eTime = endDate.value
    print str(eTime)

def updateSensor(attrname, old, new):
    global sensorList
    print sensorList[pr.active]    


text.on_change('value', update_title)
startDate.on_change('value', updateStart)
endDate.on_change('value',updateEnd)
pr.on_change('active', updateSensor)
refreshButton.on_click(updateData)

#------------------------------------------------------------------------------------
# Set up layouts and add to document
#------------------------------------------------------------------------------------

plot = plotData(df)

inputs = widgetbox(text, pr, startDate, endDate, refreshButton)
layout = row(inputs, plot, width=1000)
curdoc().add_root(layout)
curdoc().title = "16 Trafalgar Place Environmental readings"


