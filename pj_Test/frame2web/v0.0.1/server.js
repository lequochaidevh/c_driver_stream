const WebSocket = require('ws');

const wss = new WebSocket.Server({port: 9000});

let globalState = {brightness: 50, enableAI: false};

wss.on('connection', ws => {
    console.log('Client connected');

    // gửi state ban đầu
    ws.send(JSON.stringify(globalState));

    ws.on('message', msg => {
        const data = JSON.parse(msg);

        console.log('Received:', data);

        // update state
        globalState[data.id] = data.value;

        // broadcast cho tất cả client
        wss.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(JSON.stringify(globalState));
            }
        });
    });
});

console.log('WS server running at ws://localhost:9000');