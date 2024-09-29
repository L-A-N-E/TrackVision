import dash  # Importando o framework Dash para aplicações web
from dash import dcc, html  # Importando componentes principais do Dash e componentes HTML
from dash.dependencies import Input, Output, State  # Importando dependências necessárias para callbacks
import plotly.graph_objs as go  # Importando Plotly para gráficos
import requests  # Importando requests para fazer requisições HTTP

# Constantes para endereço IP, porta e host
IP_ADDRESS = "PUBLIC_IP"
PORT_STH = 8666
DASH_HOST = "0.0.0.0"

# Função para recuperar dados de voltas de um endpoint especificado
def get_lap_data(lastN):
    url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/TrackVision/id/urn:ngsi-ld:TRV:027/attributes/lap?lastN={lastN}"
    headers = {'fiware-service': 'smart', 'fiware-servicepath': '/'}
    response = requests.get(url, headers=headers)  # Fazendo uma requisição GET
    if response.status_code == 200:
        data = response.json()  # Analisando a resposta JSON
        try:
            values = data['contextResponses'][0]['contextElement']['attributes'][0]['values']  # Extraindo valores
            return values
        except KeyError:  # Tratando casos em que os dados podem não estar estruturados como esperado
            return []
    return []  # Retornando uma lista vazia se a requisição falhar

app = dash.Dash(__name__, external_stylesheets=['./assets/style.css'])  # Inicializando a aplicação Dash

# Definindo o layout da aplicação
app.layout = html.Div(children=[
    html.H1('Lap Time and Speed Viewer'),  # Título do aplicativo
    html.Div(className='graph-container', children=[
        dcc.Graph(id='lap-time-graph'),  # Componente de gráfico para exibir os tempos de volta
    ]),
    dcc.Store(id='lap-data-store', data={'current_lap': [], 'is_collecting': False}),  # Armazenando dados das voltas
    dcc.Interval(id='interval-component', interval=1*1000, n_intervals=0),  # Intervalo para atualização de dados
], style={'textAlign': 'center', 'display': 'flex', 'flexDirection': 'column', 'alignItems': 'center'})  # Layout centralizado

# Callback para atualizar o armazenamento de dados com os dados das voltas
@app.callback(
    Output('lap-data-store', 'data'),
    Input('interval-component', 'n_intervals'),
    State('lap-data-store', 'data')
)
def update_data_store(n, stored_data):
    data_lap = get_lap_data(10)  # Buscando as últimas 10 voltas

    if data_lap:
        lap_numbers = [entry['attrValue'][0] for entry in data_lap]  # Extraindo números das voltas
        lap_times = [entry['attrValue'][1] / 1000 for entry in data_lap]  # Convertendo tempos das voltas de ms para segundos

        # Contando quantos '1's estão presentes
        count_ones = lap_numbers.count(1)

        if count_ones > 0:
            # Se houver '1's, encontrar o último índice de '1'
            last_index = max(i for i, x in enumerate(lap_numbers) if x == 1)

            # Reiniciando os dados e começando a coleta a partir do último '1'
            stored_data['current_lap'] = []
            stored_data['is_collecting'] = True

            for i in range(last_index, len(lap_numbers)):
                lap_number = lap_numbers[i]
                lap_time = lap_times[i]

                if 1 <= lap_number <= 10:
                    stored_data['current_lap'].append((lap_number, lap_time))
        else:
            # Se não houver '1's, coletar todos os dados normalmente
            stored_data['current_lap'] = [(lap_numbers[i], lap_times[i]) for i in range(len(lap_numbers)) if lap_numbers[i] >= 1]
            stored_data['is_collecting'] = True  # Pode ser ajustado conforme necessário

        return stored_data  # Retorna os dados armazenados

    return stored_data  # Retorna os dados armazenados se não houver novos dados

# Callback para atualizar o gráfico com base nos dados das voltas armazenados
@app.callback(
    Output('lap-time-graph', 'figure'),
    Input('lap-data-store', 'data')
)
def update_graph(stored_data):
    if stored_data['current_lap']:
        lap_numbers, lap_times = zip(*stored_data['current_lap'])  # Desempacotando os dados das voltas
    else:
        return go.Figure()  # Retorna uma figura vazia se não houver dados

    # Criando um gráfico de dispersão para os tempos de volta
    trace_lap_time = go.Scatter(
        x=lap_numbers,
        y=lap_times,
        mode='lines+markers',
        name='Lap Time',
        line=dict(color='orange')  # Definindo a cor da linha
    )
    fig_lap_time = go.Figure(data=[trace_lap_time])  # Criando a figura
    fig_lap_time.update_layout(
        title='Lap Times',
        xaxis_title='Lap Number',  # Título do eixo x
        yaxis_title='Lap Time (seconds)',  # Título do eixo y
        hovermode='closest'  # Modo de hover mais próximo
    )

    return fig_lap_time  # Retorna a figura atualizada

# Executando a aplicação Dash
if __name__ == '__main__':
    app.run_server(debug=True, host=DASH_HOST, port=8050)  # Iniciando o servidor

