// controlLibWithCam.js
// -----------------------------
// Control Panel + Webcam helper
// -----------------------------

class ControlLib {
    constructor() {
        this.controls = {};
    }

    // -----------------------------
    // UI Controls
    // -----------------------------
    addButton(id, callback) {
        const el = document.getElementById(id);
        if(!el) return;
        el.addEventListener('click', callback);
        this.controls[id] = { type: 'button', el: el };
    }

    addSlider(id, labelId=null) {
        const el = document.getElementById(id);
        if(!el) return;
        const labelEl = labelId ? document.getElementById(labelId) : null;
        if(labelEl) labelEl.textContent = el.value;
        el.addEventListener('input', () => {
            if(labelEl) labelEl.textContent = el.value;
        });
        this.controls[id] = { type: 'slider', el: el };
    }

    addSwitch(id) {
        const el = document.getElementById(id);
        if(!el) return;
        this.controls[id] = { type: 'switch', el: el };
    }

    // -----------------------------
    // JSON Handling
    // -----------------------------
    saveState() {
        const state = {};
        for(const key in this.controls) {
            const ctrl = this.controls[key];
            switch(ctrl.type) {
                case 'slider': state[key] = parseFloat(ctrl.el.value); break;
                case 'switch': state[key] = ctrl.el.checked; break;
                case 'button': break;
            }
        }
        return state;
    }

    loadState(state) {
        for(const key in state) {
            if(this.controls[key]) {
                const ctrl = this.controls[key];
                switch(ctrl.type) {
                    case 'slider': 
                        ctrl.el.value = state[key];
                        const labelEl = document.querySelector(`[for="${key}"] + span`);
                        if(labelEl) labelEl.textContent = state[key];
                        break;
                    case 'switch':
                        ctrl.el.checked = state[key];
                        break;
                }
            }
        }
    }

    loadJsonFromFile(inputEl) {
        return new Promise((resolve, reject) => {
            const file = inputEl.files[0];
            if(!file) return reject("No file selected");
            const reader = new FileReader();
            reader.onload = e => {
                try {
                    const state = JSON.parse(e.target.result);
                    this.loadState(state);
                    resolve(state);
                } catch(err) {
                    reject(err);
                }
            };
            reader.readAsText(file);
        });
    }

    saveJsonToFile(filename="config.json") {
        const state = this.saveState();
        const blob = new Blob([JSON.stringify(state, null, 2)], {type: "application/json"});
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = filename;
        a.click();
        URL.revokeObjectURL(url);
    }

    // -----------------------------
    // Webcam helper
    // -----------------------------
    // videoEl: <video> element
    // options: { deviceId: string | undefined, width, height }
    async startWebcam(videoEl, options={}) {
        if(!videoEl) throw new Error("Video element not provided");
        const constraints = {
            video: {
                width: options.width || 640,
                height: options.height || 480
            }
        };
        if(options.deviceId) {
            constraints.video.deviceId = { exact: options.deviceId };
        }

        try {
            const stream = await navigator.mediaDevices.getUserMedia(constraints);
            videoEl.srcObject = stream;
            await videoEl.play();
            return stream;
        } catch(err) {
            console.error("Không thể mở webcam:", err);
            throw err;
        }
    }

    // Stop webcam stream
    stopWebcam(videoEl) {
        if(videoEl && videoEl.srcObject) {
            const tracks = videoEl.srcObject.getTracks();
            tracks.forEach(track => track.stop());
            videoEl.srcObject = null;
        }
    }
}

// Singleton instance
const controlLib = new ControlLib();