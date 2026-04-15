
function drawBox(ctx, canvas, x, y, bw, bh, label = '', normalize = true) {
    let drawX = x;
    let drawY = y;
    let drawW = bw;
    let drawH = bh;

    // ---------------- NORMALIZE ----------------
    if (normalize) {
        drawX = x * canvas.width;
        drawY = y * canvas.height;
        drawW = bw * canvas.width;
        drawH = bh * canvas.height;
    }

    // ---------------- STYLE ----------------
    ctx.strokeStyle = 'red';
    ctx.lineWidth   = 2;
    ctx.font        = '16px Arial';
    ctx.fillStyle   = 'red';

    // ---------------- DRAW RECT ----------------
    ctx.strokeRect(drawX, drawY, drawW, drawH);

    // ---------------- LABEL ----------------
    if (label) {
        const padding   = 4;
        const textWidth = ctx.measureText(label).width;

        // background cho label
        ctx.fillRect(drawX, drawY - 20, textWidth + padding * 2, 20);

        // text
        ctx.fillStyle = 'white';
        ctx.fillText(label, drawX + padding, drawY - 5);

        ctx.fillStyle = 'red';  // reset
    }
}

// window.OverlayLib = {
//     drawBox(ctx, canvas, x, y, w, h, label = '', normalize = true) {
//         if (normalize) {
//             x *= canvas.width;
//             y *= canvas.height;
//             w *= canvas.width;
//             h *= canvas.height;
//         }

//         ctx.strokeStyle = 'red';
//         ctx.lineWidth   = 2;
//         ctx.strokeRect(x, y, w, h);

//         if (label) {
//             ctx.fillStyle = 'red';
//             ctx.fillText(label, x, y - 5);
//         }
//     }
// };