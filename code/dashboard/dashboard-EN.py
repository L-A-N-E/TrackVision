import dash  # Importing Dash framework for web applications
from dash import dcc, html  # Importing Dash core components and HTML components
from dash.dependencies import Input, Output, State  # Importing necessary dependencies for callbacks
import plotly.graph_objs as go  # Importing Plotly for graphing
import requests  # Importing requests for HTTP requests

# Constants for IP address, port, and host
IP_ADDRESS = "PUBLIC_IP"
PORT_STH = 8666
DASH_HOST = "0.0.0.0"

# Function to retrieve lap data from a specified endpoint
def get_lap_data(lastN):
    url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/TrackVision/id/urn:ngsi-ld:TRV:027/attributes/lap?lastN={lastN}"
    headers = {'fiware-service': 'smart', 'fiware-servicepath': '/'}
    response = requests.get(url, headers=headers)  # Making a GET request
    if response.status_code == 200:
        data = response.json()  # Parsing JSON response
        try:
            values = data['contextResponses'][0]['contextElement']['attributes'][0]['values']  # Extracting values
            return values
        except KeyError:  # Handling cases where data may not be structured as expected
            return []
    return []  # Returning empty list if request failed

app = dash.Dash(__name__, external_stylesheets=['./assets/style.css'])  # Initializing the Dash application

# Defining the layout of the application
app.layout = html.Div(children=[
    html.H1('Lap Time and Speed Viewer'),  # Title of the app
    html.Div(className='graph-container', children=[
        dcc.Graph(id='lap-time-graph'),  # Graph component for displaying lap times
    ]),
    dcc.Store(id='lap-data-store', data={'current_lap': [], 'is_collecting': False}),  # Store for lap data
    dcc.Interval(id='interval-component', interval=1*1000, n_intervals=0),  # Interval for data update
], style={'textAlign': 'center', 'display': 'flex', 'flexDirection': 'column', 'alignItems': 'center'})  # Centered layout

# Callback to update the data store with lap data
@app.callback(
    Output('lap-data-store', 'data'),
    Input('interval-component', 'n_intervals'),
    State('lap-data-store', 'data')
)
def update_data_store(n, stored_data):
    data_lap = get_lap_data(10)  # Fetching the last 10 laps

    if data_lap:
        lap_numbers = [entry['attrValue'][0] for entry in data_lap]  # Extracting lap numbers
        lap_times = [entry['attrValue'][1] / 1000 for entry in data_lap]  # Converting lap times from ms to seconds

        # Counting how many '1's are present
        count_ones = lap_numbers.count(1)

        if count_ones > 0:
            # If there are '1's, find the last index of '1'
            last_index = max(i for i, x in enumerate(lap_numbers) if x == 1)

            # Resetting data and starting collection from the last '1'
            stored_data['current_lap'] = []
            stored_data['is_collecting'] = True

            for i in range(last_index, len(lap_numbers)):
                lap_number = lap_numbers[i]
                lap_time = lap_times[i]

                if 1 <= lap_number <= 10:
                    stored_data['current_lap'].append((lap_number, lap_time))
        else:
            # If there are no '1's, collect all data normally
            stored_data['current_lap'] = [(lap_numbers[i], lap_times[i]) for i in range(len(lap_numbers)) if lap_numbers[i] >= 1]
            stored_data['is_collecting'] = True  # May be adjusted as needed

        return stored_data

    return stored_data  # Return stored data if no new data

# Callback to update the graph based on the stored lap data
@app.callback(
    Output('lap-time-graph', 'figure'),
    Input('lap-data-store', 'data')
)
def update_graph(stored_data):
    if stored_data['current_lap']:
        lap_numbers, lap_times = zip(*stored_data['current_lap'])  # Unpacking lap data
    else:
        return go.Figure()  # Return an empty figure if no data

    # Creating a scatter plot for lap times
    trace_lap_time = go.Scatter(
        x=lap_numbers,
        y=lap_times,
        mode='lines+markers',
        name='Lap Time',
        line=dict(color='orange')
    )
    fig_lap_time = go.Figure(data=[trace_lap_time])  # Creating the figure
    fig_lap_time.update_layout(
        title='Lap Times',
        xaxis_title='Lap Number',
        yaxis_title='Lap Time (seconds)',
        hovermode='closest'
    )

    return fig_lap_time  # Return the updated figure

# Running the Dash application
if __name__ == '__main__':
    app.run_server(debug=True, host=DASH_HOST, port=8050)
