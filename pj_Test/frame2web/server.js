// server.js

const WebSocket = require('ws');

const wss = new WebSocket.Server({port: 9000});

console.log('WS server running at ws://localhost:9000');

// wss.on('connection', (ws) => {
//     console.log('Client connected');

//     const interval = setInterval(() => {
//         // random box demo
//         const data = {
//             boxes: [{
//                 x: Math.random() * 0.6,
//                 y: Math.random() * 0.6,
//                 w: 0.2,
//                 h: 0.2,
//                 label: 'AI'
//             }]
//         };

//         ws.send(JSON.stringify(data));
//     }, 300);

//     ws.on('close', () => {
//         clearInterval(interval);
//         console.log('Client disconnected');
//     });
// });

wss.on('connection', (ws) => {
    console.log('Client connected');

    ws.on('message', (msg) => {
        // broadcast cho tất cả client
        wss.clients.forEach(client => {
            if (client.readyState === 1) {
                client.send(msg.toString());
            }
        });
    });
});