// overlayLib.js

window.OverlayLib = {
    drawBox(ctx, canvas, x, y, w, h, label = '', normalize = true) {
        if (normalize) {
            x *= canvas.width;
            y *= canvas.height;
            w *= canvas.width;
            h *= canvas.height;
        }

        ctx.strokeStyle = 'red';
        ctx.lineWidth   = 2;
        ctx.font        = '16px Arial';

        ctx.strokeRect(x, y, w, h);

        if (label) {
            const padding   = 4;
            const textWidth = ctx.measureText(label).width;

            ctx.fillStyle = 'red';
            ctx.fillRect(x, y - 20, textWidth + padding * 2, 20);

            ctx.fillStyle = 'white';
            ctx.fillText(label, x + padding, y - 5);
        }
    },

    drawBoxes(ctx, canvas, boxes, normalize = true) {
        boxes.forEach(b => {
            this.drawBox(ctx, canvas, b.x, b.y, b.w, b.h, b.label, normalize);
        });
    }
};